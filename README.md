# Edge Detectr Backend

The backend service for Edge Detectr, providing image processing capabilities using various edge detection operators.

## Features

- Multiple edge detection operators:
  - OpenCV Sobel
  - Alternative Sobel
  - OpenMP Sobel (parallel processing)
  - Prewitt
  - Roberts Cross
- Automatic file cleanup
- RESTful API endpoints
- Docker containerization

## Prerequisites

- Docker and Docker Compose
- Node.js 18.x (for local development)
- OpenCV libraries
- C++ build tools

## Project Structure

```
backend/
├── src/
│   ├── app.js              # Main application entry
│   └── routes/
│       └── operators.js    # Image processing routes
├── operators/
│   ├── main.cpp           # C++ operator implementations
│   └── CMakeLists.txt     # CMake configuration
├── uploads/               # Temporary upload storage
├── results/              # Processed images storage
└── Dockerfile            # Container configuration
```

## API Endpoints

### POST `/api/operators/:operator`

Process an image with the specified edge detection operator.

- Parameters:
  - `operator`: The edge detection algorithm to use
  - `file`: The image file to process (multipart/form-data)
- Returns:
  - `inputImage`: Original image filename
  - `outputImage`: Processed image filename

### GET `/uploads/:filename`

Retrieve an uploaded image.

### GET `/results/:filename`

Retrieve a processed image.

## Environment Variables

- `OPERATOR_PROCESS`: Path to the operator executable (default: `/app/operators/build`)
- `NODE_ENV`: Environment mode (development/production)
- `PORT`: Server port (default: 3001)

## Development

1. Install dependencies:
   ```bash
   npm install
   ```

2. Build the C++ operators:
   ```bash
   cd operators
   mkdir build && cd build
   cmake ..
   make
   ```

3. Start the development server:
   ```bash
   npm run dev
   ```

## Docker Deployment

1. Build and start the container:
   ```bash
   docker compose up --build backend
   ```

2. The service will be available at `http://localhost:3001`

## File Cleanup

The service automatically cleans up uploaded and processed images:
- When a new image is uploaded
- Files are stored in `uploads/` and `results/` directories
- Cleanup is handled by the `cleanFolders()` function in `operators.js` 