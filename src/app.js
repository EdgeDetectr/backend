const express = require("express");
const PORT = 3001;
const cors = require("cors");
const path = require("path");

const operatorRoutes = require("./routes/operators");

const app = express();

app.use(cors());

// dummy entry route
app.get("/", (req, res) => {
  res.status(200);
  res.send("Welcome to root URL of Server");
});

// API routes for operators
app.use("/api/operators", operatorRoutes);

// Upload folder and route
const uploadFolder = path.join(__dirname, "../uploads");
app.use("/uploads", express.static(uploadFolder));

// API route to list all uploaded files
app.get("/api/uploads", (req, res) => {
  fs.readdir(uploadFolder, (err, files) => {
    if (err) {
      return res
        .status(500)
        .json({ error: "Failed to read uploads directory" });
    }
    res.json({ files });
  });
});

// app listening on PORT
app.listen(PORT, (error) => {
  if (!error)
    console.log(
      "Server is Successfully Running, and App is listening on port " + PORT
    );
  else console.log("Error occurred, server can't start", error);
});
