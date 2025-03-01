const express = require("express");
const PORT = 3001;
const cors = require("cors");
const path = require("path");
const fs = require("fs");

const operatorRoutes = require("./routes/operators");

const app = express();

app.use(cors());

// dummy entry route
app.get("/", (req, res) => {
  res.status(200);
  res.send("Welcome to root URL of Server");
});

// Health check endpoint for load balancer
app.get("/health", (req, res) => {
  res.status(200).json({ status: "healthy" });
});

// API routes for operators
app.use("/api/operators", operatorRoutes);

// Upload folder and route
const uploadFolder = path.join(__dirname, "../uploads");
app.use("/uploads", express.static(uploadFolder));

// Results folder and route
const resultsFolder = path.join(__dirname, "../results");
app.use("/results", express.static(resultsFolder));

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
