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
    ];

// CORS configuration
const corsOptions = {
  origin: function (origin, callback) {
    // Log all incoming requests for debugging
    console.log("Incoming request origin:", origin);
    console.log("Current environment:", process.env.NODE_ENV);
    console.log("Allowed origins:", allowedOrigins);

    // Allow requests with no origin (like mobile apps, curl, etc.)
    if (!origin) {
      console.log("No origin provided, allowing request");
      return callback(null, true);
    }

    // For development, allow all origins
    if (process.env.NODE_ENV !== "production") {
      console.log("Development environment, allowing all origins");
      return callback(null, true);
    }

    // Check if origin is allowed
    if (allowedOrigins.includes(origin)) {
      console.log("Origin allowed:", origin);
      return callback(null, true);
    }

    console.log("Origin rejected:", origin);
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
  maxAge: 86400, // 24 hours
  preflightContinue: false,
  optionsSuccessStatus: 204,
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
