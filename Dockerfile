FROM ubuntu:latest

# 1. System Dependencies (Change rarely)
# Set noninteractive to prevent apt from hanging on prompts
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    "tar" \
    "xz-utils" \
    "curl" \
    "ca-certificates" \
    "git" \
    "pkg-config" \
    "zip" \
    "unzip" \
    "build-essential" \
    && apt-get clean -y \
    && rm -fr /var/lib/apt/lists

# 2. Install Custom CMake (Changes rarely)
# We do this early so we don't re-download it if vcpkg or source code changes.
ARG CMAKE_VERSION="3.31.10" 

RUN base_url="https://github.com/Kitware/CMake/releases/download" \
    && url="${base_url}/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz" \
    && mkdir -p /opt/cmake \
    && (curl -sL "$url" | tar --strip-components=1 -xz -C /opt/cmake) \
    && rm -fr /opt/cmake/bin/ccmake /opt/cmake/bin/cmake-gui
ENV PATH="/opt/cmake/bin:${PATH}"

# 3. Setup Vcpkg Tooling (Changes occasionally)
ENV VCPKG_ROOT=/opt/vcpkg
ENV CMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

RUN git clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}" \
    && "${VCPKG_ROOT}/bootstrap-vcpkg.sh"

# 4. Install Project Dependencies (Changes when you add libraries)
# We copy ONLY the dependency manifest first. 
# This is the most important optimization: if you change main.cpp, 
# Docker will skip this step and use the cached version of 'fmt'.
WORKDIR /app
COPY vcpkg.json .

# Installing via manifest (vcpkg.json) is cleaner than manual install
# If you don't use vcpkg.json, run: RUN "${VCPKG_ROOT}/vcpkg" install fmt
# RUN "${VCPKG_ROOT}/vcpkg" install

# 5. Project Source Code (Changes frequently)
# This goes last. When you edit main.cpp, Docker only rebuilds from here down.
# COPY CMakeLists.txt .
# COPY main.cpp .

# 6. Build
# RUN cmake . -B build -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
# RUN cmake --build build

# 7. Runtime
CMD ["sh", "-c", "echo 'Hello from Docker!' && which cmake && cmake --version"]
