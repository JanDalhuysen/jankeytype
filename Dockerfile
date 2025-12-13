FROM ubuntu:latest

# 1. System Dependencies
# Combine updates and installs to keep layers small and clean up afterwards
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    curl \
    git \
    pkg-config \
    tar \
    unzip \
    xz-utils \
    zip \
    # Raylib specific dependencies
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    libwayland-dev \
    libxkbcommon-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 2. Install Custom CMake (Changes rarely)
ARG CMAKE_VERSION="3.31.10"
RUN base_url="https://github.com/Kitware/CMake/releases/download" \
    && url="${base_url}/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz" \
    && mkdir -p /opt/cmake \
    && (curl -sL "$url" | tar --strip-components=1 -xz -C /opt/cmake) \
    && rm -rf /opt/cmake/bin/ccmake /opt/cmake/bin/cmake-gui
ENV PATH="/opt/cmake/bin:${PATH}"

# 3. Setup Vcpkg (Changes occasionally)
ENV VCPKG_ROOT=/opt/vcpkg
ENV CMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
RUN git clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}" \
    && "${VCPKG_ROOT}/bootstrap-vcpkg.sh"

# 4. Install Project Dependencies (Cached Layer)
WORKDIR /app
COPY vcpkg.json .

# Pre-install dependencies so they are cached if main.cpp changes
RUN "${VCPKG_ROOT}/vcpkg" install

# 5. Build Project
COPY CMakeLists.txt .
COPY main.cpp .

# Configure and Build
RUN cmake . -B build -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
RUN cmake --build build

# 6. Default Command
# If you run the container, it prints the version, but mostly we use this to extract artifacts
CMD ["./build/jankeytype"]
