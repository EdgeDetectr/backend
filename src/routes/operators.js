const express = require("express");
const multer = require("multer");
const { processSobel } = require("../controllers/operators.js");
const router = express.Router();

const upload = multer({ dest: "uploads/" });

router.post("/sobel", upload.single("image"), processSobel);

module.exports = router;
