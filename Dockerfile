# FROM --platform=linux/amd64 node:18-slim

# WORKDIR /app

# COPY package*.json ./
# RUN npm install --production

# RUN apt-get update && apt-get install -y \
#     build-essential \
#     libopencv-dev \
#     libomp-dev \
#     libssl-dev \
#     pkg-config \
#     && rm -rf /var/lib/apt/lists/* \
#     cmake \
#     && rm -rf /var/lib/apt/lists/*

# RUN apt-get update && apt-get install -y \
# wget \
# && wget -q https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-x86_64.sh \
# && chmod +x cmake-3.30.0-linux-x86_64.sh \
# && ./cmake-3.30.0-linux-x86_64.sh --skip-license --prefix=/usr/local \
# && rm cmake-3.30.0-linux-x86_64.sh

# ENV CC=/usr/bin/gcc
# ENV CXX=/usr/bin/g++
    
# COPY ./operators/ ./operators/
# WORKDIR /app/operators
# RUN rm -rf build && \
#     mkdir -p build && \
#     cd build && \
#     cmake .. -DCMAKE_SYSTEM_NAME=Linux && \
#     make

# WORKDIR /app
# COPY . .

# EXPOSE 3001

# CMD ["node", "src/app.js"]

# Use Ubuntu for full C++ and AWS support
FROM --platform=$TARGETPLATFORM ubuntu:22.04 AS build

# Set environment variables to prevent timezone prompt
ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC

WORKDIR /app

# Install system dependencies for C++ compilation
RUN apt-get update && apt-get install -y \
    curl \
    build-essential \
    cmake \
    libopencv-dev \
    libomp-dev \
    libssl-dev \
    pkg-config \
    tzdata \
    && rm -rf /var/lib/apt/lists/*

# Install CMake 3.30 manually
RUN curl -fsSL https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-x86_64.sh -o cmake-install.sh && \
    chmod +x cmake-install.sh && \
    ./cmake-install.sh --skip-license --prefix=/usr/local && \
    rm cmake-install.sh

# Set compiler variables
ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++

# Copy C++ operators
COPY ./operators/ ./operators/
WORKDIR /app/operators

# Ensure correct architecture compilation
ARG TARGETPLATFORM
RUN echo "Building for platform: $TARGETPLATFORM" && \
    rm -rf build && \
    mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_SYSTEM_NAME=Linux && \
    make

# Use node image for final runtime
FROM --platform=$TARGETPLATFORM node:18-slim AS runtime

WORKDIR /app

# Copy compiled C++ binary from the build stage
COPY --from=build /app/operators/build/operators /app/operators/build/operators

# Install dependencies
COPY package*.json ./
RUN npm install --production

# Copy the entire project
COPY . .

# Expose backend port
EXPOSE 3001

# Start the Express.js server
CMD ["node", "src/app.js"]
