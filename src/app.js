const express = require("express");
const PORT = 3001;
const cors = require("cors");
const path = require("path");
const fs = require("fs");

const operatorRoutes = require("./routes/operators");

const app = express();

// Configure CORS with environment variables or defaults for local development
const allowedOrigins = process.env.ALLOWED_ORIGINS
  ? process.env.ALLOWED_ORIGINS.split(",")
  : [
      "http://localhost:3000",
      "https://www.edgedetectr.com",
      "https://edgedetectr.com",
      "https://edgedetectr-lb-2106112805.us-east-1.elb.amazonaws.com",
    ];

// CORS configuration
const corsOptions = {
  origin: ["https://api.edgedetectr.com"],
  methods: "GET,POST,PUT,DELETE",
  allowedHeaders: "Content-Type,Authorization",
};

// Enable CORS for all routes
app.use(cors(corsOptions));

// Add preflight handling for all routes
app.options("*", cors(corsOptions));

// Add error handling for CORS errors
app.use((err, req, res, next) => {
  if (err.message.includes("CORS")) {
    console.error("CORS Error:", err);
    console.error("Request headers:", req.headers);
    console.error("Request origin:", req.headers.origin);

    return res.status(403).json({
      error: "CORS Error",
      message: err.message,
      allowedOrigins:
        process.env.NODE_ENV === "production" ? "hidden" : allowedOrigins,
      requestOrigin: req.headers.origin,
    });
  }
  next(err);
});

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
