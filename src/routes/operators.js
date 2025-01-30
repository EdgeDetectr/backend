const express = require("express");
const multer = require("multer");
const cors = require("cors");
const path = require("path");
const { spawn } = require("child_process");
const fs = require("fs");
const router = express.Router();

router.use(cors());

// Upload folder
const uploadFolder = path.join(__dirname, "../../uploads");
// Upload multer with unique filename and folder creation if does not exist
const upload = multer({
  storage: multer.diskStorage({
    destination: (req, file, cb) => {
      cb(null, uploadFolder);
    },
    filename: (req, file, cb) => {
      const uniqueFilename = `${Date.now()}-${file.originalname}`;
      cb(null, uniqueFilename);
    },
  }),
});

// POST /operators/:operator route to process the uploaded image with the operator and saving the output image
router.post("/:operator", upload.single("file"), (req, res) => {
  const operator = req.params.operator;
  console.log("Operator:", operator);
  const encodedOperator = encodeURIComponent(operator);
  const inputFilename = req.file.filename;
  const outputFilename = `output-${inputFilename}`;

  const inputPath = path.join(uploadFolder, inputFilename);
  const outputPath = path.join(uploadFolder, outputFilename);

  if (!fs.existsSync(inputPath)) {
    return res.status(400).json({ error: "No file uploaded." });
  }

  const executablePath =
    process.env.OPERATOR_PROCESS || __dirname + "/../../operators/build";

  console.log(process.env.OPERATOR_PROCESS);

  const operatorProcess = path.join(executablePath, "operators");

  console.log("Processing image with operator:", operator);
  console.log("Input file:", inputPath);
  console.log("Output file:", outputPath);
  console.log("Executable path:", executablePath);
  console.log("Operator process:", operatorProcess);

  if (!fs.existsSync(operatorProcess)) {
    console.error("Operator executable not found at:", operatorProcess);
    return res.status(500).json({ error: "Operator executable not found." });
  }

  try {
    process.chdir(executablePath);
  } catch (err) {
    console.error(
      "Failed to change directory:",
      err,
      "current directory:",
      __dirname,
      "executable path:",
      executablePath
    );
    return res.status(500).json({ error: "Internal server error." });
  }

  const cppProcess = spawn(operatorProcess, [
    encodedOperator,
    inputPath,
    outputPath,
  ]);

  cppProcess.stdout.on("data", (data) => {
    console.log(`stdout: ${data}`);
  });

  cppProcess.stderr.on("data", (data) => {
    console.error(`stderr: ${data}`);
  });

  cppProcess.on("close", (code) => {
    console.log("C++ process closed with code:", code);
    process.chdir(__dirname);

    if (code !== 0) {
      console.error(`C++ process exited with code ${code}`);
      return res.status(500).json({ error: "Processing failed." });
    }

    console.log(inputFilename);
    console.log(outputFilename);
    res.json({
      inputImage: inputFilename,
      outputImage: outputFilename,
    });

    setTimeout(() => {
      try {
        fs.unlinkSync(inputPath);
        fs.unlinkSync(outputPath);
      } catch (cleanupErr) {
        console.error("Error cleaning up files:", cleanupErr);
      }
    }, 60000);
  });

  cppProcess.on("error", (err) => {
    console.error("C++ process error:", err);
    process.chdir(__dirname);
    console.error("Failed to start subprocess:", err);
    return res.status(500).json({ error: "Failed to start subprocess." });
  });
});

module.exports = router;
