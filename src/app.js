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
  : ["http://localhost:3000"];

app.use(
  cors({
    origin: function (origin, callback) {
      // Allow requests with no origin (like mobile apps, curl, etc.)
      if (!origin) return callback(null, true);

      // For preflight requests, always allow during development and testing
      // Production should rely on environment variables
      if (
        process.env.NODE_ENV !== "production" ||
        allowedOrigins.includes(origin)
      ) {
        return callback(null, true);
      }

      console.log(
        "Origin rejected:",
        origin,
        "Allowed origins:",
        allowedOrigins
      );
      return callback(new Error(`Origin ${origin} not allowed by CORS`), false);
    },
    credentials: true,
    methods: ["GET", "POST", "PUT", "DELETE", "OPTIONS"],
    allowedHeaders: ["Content-Type", "Authorization"],
  })
);

// Add preflight handling for all routes
app.options("*", cors());

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
