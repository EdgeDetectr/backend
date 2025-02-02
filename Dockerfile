# Build stage for C++ operators
FROM ubuntu:22.04 AS cpp-builder

# Prevent interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC

WORKDIR /app

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev \
    libomp-dev \
    libssl-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Copy and build C++ operators
COPY ./operators/ ./operators/
WORKDIR /app/operators

# Clean build directory and rebuild with static linking
RUN rm -rf build && \
    mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++" \
          .. && \
    make -j$(nproc)

# Node.js runtime stage
FROM ubuntu:22.04 AS runtime

WORKDIR /app

# Set timezone and prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC

# Install Node.js
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && \
    apt-get update && \
    apt-get install -y curl && \
    curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs && \
    rm -rf /var/lib/apt/lists/*

# Install runtime dependencies
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && \
    apt-get update && apt-get install -y \
    libopencv-dev \
    libomp5 \
    && rm -rf /var/lib/apt/lists/*

# Create necessary directories
RUN mkdir -p /app/operators/build && \
    mkdir -p /app/uploads && \
    mkdir -p /app/results && \
    chmod 777 /app/uploads && \
    chmod 777 /app/results

# Copy compiled C++ operators
COPY --from=cpp-builder /app/operators/build/operators /app/operators/build/operators
RUN chmod +x /app/operators/build/operators

# Install Node.js dependencies
COPY package*.json ./
RUN npm install --production

# Copy application code
COPY . .

EXPOSE 3001

CMD ["node", "src/app.js"]
