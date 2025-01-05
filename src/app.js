const express = require("express");
const PORT = 3001;
const cors = require("cors");

const operatorRoutes = require("./routes/operators");

const app = express();

app.use(cors());

app.get("/", (req, res) => {
  res.status(200);
  res.send("Welcome to root URL of Server");
});

app.use("/api/operators", operatorRoutes);

app.listen(PORT, (error) => {
  if (!error)
    console.log(
      "Server is Successfully Running, and App is listening on port " + PORT
    );
  else console.log("Error occurred, server can't start", error);
});
