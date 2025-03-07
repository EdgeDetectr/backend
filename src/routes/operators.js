const express = require("express");
const multer = require("multer");
const cors = require("cors");
const path = require("path");
const { spawn } = require("child_process");
const fs = require("fs");
const router = express.Router();

// Configure CORS with environment variables or defaults for local development
const allowedOrigins = process.env.ALLOWED_ORIGINS
  ? process.env.ALLOWED_ORIGINS.split(",")
  : [
      "http://localhost:3000",
      "https://www.edgedetectr.com",
      "https://edgedetectr.com",
      "https://api.edgedetectr.com",
      "https://edgedetectr-lb-2106112805.us-east-1.elb.amazonaws.com",
    ];

router.use(
  cors({
    origin: function (origin, callback) {
      // console.log("Operator route - Incoming request origin:", origin);

      // Allow requests with no origin (like mobile apps, curl, etc.)
      if (!origin) {
        // console.log("No origin provided, allowing request");
        return callback(null, true);
      }

      // For development, allow all origins
      if (process.env.NODE_ENV !== "production") {
        // console.log("Development environment, allowing all origins");
        return callback(null, true);
      }

      // Check if origin is allowed
      if (
        allowedOrigins.includes(origin) ||
        origin.endsWith("edgedetectr.com")
      ) {
        // console.log("Origin allowed:", origin);
        return callback(null, true);
      }

      // console.log("Origin rejected:", origin);
      return callback(new Error(`Origin ${origin} not allowed by CORS`), false);
    },
    credentials: true,
    methods: ["GET", "POST", "PUT", "DELETE", "OPTIONS"],
    allowedHeaders: [
      "Content-Type",
      "Authorization",
      "Accept",
      "Origin",
      "X-Requested-With",
    ],
    exposedHeaders: ["Content-Range", "X-Content-Range"],
    maxAge: 86400,
  })
);

// Add preflight handling for all routes
router.options("*", cors());

// Upload folder
const uploadFolder = path.join(__dirname, "../../uploads");
const resultsFolder = path.join(__dirname, "../../results");

// Function to clean folders
const cleanFolders = () => {
  try {
    // Create directories if they don't exist
    if (!fs.existsSync(uploadFolder)) {
      fs.mkdirSync(uploadFolder, { recursive: true });
      // console.log("Created uploads folder");
    }

    if (!fs.existsSync(resultsFolder)) {
      fs.mkdirSync(resultsFolder, { recursive: true });
      // console.log("Created results folder");
    }

    // Read and remove all files in uploads folder
    const uploadFiles = fs.readdirSync(uploadFolder);
    for (const file of uploadFiles) {
      fs.unlinkSync(path.join(uploadFolder, file));
    }

    // Read and remove all files in results folder
    const resultFiles = fs.readdirSync(resultsFolder);
    for (const file of resultFiles) {
      fs.unlinkSync(path.join(resultsFolder, file));
    }

    // console.log("Cleaned uploads and results folders");
  } catch (err) {
    console.error("Error cleaning folders:", err);
  }
};

// Upload multer with unique filename and folder creation if does not exist
const upload = multer({
  storage: multer.diskStorage({
    destination: (req, file, cb) => {
      // Clean folders before new upload
      cleanFolders();
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
  // console.log("Operator:", operator);
  const encodedOperator = encodeURIComponent(operator);

  // Check if file was uploaded
  if (!req.file) {
    console.error("No file was uploaded");
    return res
      .status(400)
      .json({ error: "No file was uploaded. Please select a file." });
  }

  const inputFilename = req.file.filename;
  const outputFilename = `output-${inputFilename}`;

  const inputPath = path.join(uploadFolder, inputFilename);
  const outputPath = path.join(resultsFolder, outputFilename);

  if (!fs.existsSync(inputPath)) {
    console.error("File uploaded but not found at:", inputPath);
    return res.status(400).json({
      error: "File was uploaded but cannot be found. Please try again.",
    });
  }

  // Log file details
  // console.log("Input file details:", {
  //   name: req.file.originalname,
  //   size: req.file.size,
  //   mimetype: req.file.mimetype,
  //   path: inputPath,
  // });

  const executablePath =
    process.env.OPERATOR_PROCESS ||
    path.join(__dirname, "../../operators/build");

  // console.log("Process env:", process.env.OPERATOR_PROCESS);

  const operatorProcess = path.join(executablePath, "operators");

  // console.log("Processing image with operator:", operator);
  // console.log("Input file:", inputPath);
  // console.log("Output file:", outputPath);
  // console.log("Executable path:", executablePath);
  // console.log("Operator process:", operatorProcess);

  // Check if executable exists
  if (!fs.existsSync(operatorProcess)) {
    console.error("Operator executable not found at:", operatorProcess);
    return res.status(500).json({ error: "Operator executable not found." });
  }

  // Check if input file is a valid image by checking file size and mime type
  if (req.file.size <= 0) {
    console.error("Invalid image: File is empty");
    return res.status(400).json({ error: "Invalid image file: File is empty" });
  }

  try {
    // Change to executable directory
    const currentDir = process.cwd();
    // console.log("Current directory before change:", currentDir);
    process.chdir(executablePath);
    // console.log("Directory changed to:", process.cwd());
  } catch (err) {
    console.error(
      "Failed to change directory:",
      err,
      "current directory:",
      __dirname,
      "executable path:",
      executablePath
    );
    return res
      .status(500)
      .json({ error: "Internal server error: directory change failed." });
  }

  // Collect stderr output
  let stderrOutput = "";
  let stdoutOutput = "";

  const cppProcess = spawn(operatorProcess, [
    encodedOperator,
    inputPath,
    outputPath,
  ]);

  cppProcess.stdout.on("data", (data) => {
    const output = data.toString();
    stdoutOutput += output;
    // console.log(`stdout: ${output}`);
  });

  cppProcess.stderr.on("data", (data) => {
    const error = data.toString();
    stderrOutput += error;
    console.error(`stderr: ${error}`);
  });

  cppProcess.on("close", (code) => {
    // console.log("C++ process closed with code:", code);

    // Change back to original directory
    try {
      process.chdir(__dirname);
      // console.log("Directory changed back to:", process.cwd());
    } catch (err) {
      console.error("Failed to change back to original directory:", err);
    }

    if (code !== 0) {
      console.error(`C++ process exited with code ${code}`);

      // Check if output file exists despite error
      if (fs.existsSync(outputPath)) {
        // console.log("Output file exists despite error code. Proceeding...");
      } else {
        return res.status(500).json({
          error: "Processing failed",
          details: stderrOutput || "No error details available",
          exitCode: code,
        });
      }
    }

    // Check if output file exists
    if (!fs.existsSync(outputPath)) {
      console.error("Output file not created:", outputPath);
      return res.status(500).json({
        error: "Output file not created",
        details: stderrOutput || "No error details available",
      });
    }

    // console.log("Input filename:", inputFilename);
    // console.log("Output filename:", outputFilename);
    res.json({
      inputImage: inputFilename,
      outputImage: outputFilename,
    });
  });

  cppProcess.on("error", (err) => {
    console.error("C++ process error:", err);

    // Change back to original directory
    try {
      process.chdir(__dirname);
    } catch (dirErr) {
      console.error("Failed to change back to original directory:", dirErr);
    }

    console.error("Failed to start subprocess:", err);
    return res.status(500).json({
      error: "Failed to start subprocess",
      details: err.message,
    });
  });
});

module.exports = router;
