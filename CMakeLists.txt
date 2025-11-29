cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_ROOT}
   AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "VCPKG_ROOT is set to: $ENV{VCPKG_ROOT}")
  message(STATUS "Setting CMAKE_TOOLCHAIN_FILE to use vcpkg...")
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

project(jankeytype)

find_package(raylib CONFIG QUIET)

if(NOT raylib_FOUND)
  find_package(raylib QUIET)
endif()

if(NOT raylib_FOUND)
  message(
    FATAL_ERROR
      "\n"
      "Error: raylib not found!\n"
      "We couldn't find the `raylib` library.\n"
      "Please install it by following one of these methods:\n"
      "  1. Install via package manager or from source:\n"
      "  2. Install via vcpkg: `vcpkg install raylib`\n"
      "If using vcpkg, you need to set up the `VCPKG_ROOT` environment variable:\n"
      "  Windows:\n"
      "    set VCPKG_ROOT=C:/path/to/vcpkg\n"
      "  Linux/macOS:\n"
      "    export VCPKG_ROOT=/path/to/vcpkg\n"
      "After setting `VCPKG_ROOT`, make sure to specify the vcpkg toolchain file:\n"
      "You can do this in two ways:\n"
      "  1. Command-line argument:\n"
      "    Windows:\n"
      "      cmake . -G \"MinGW Makefiles\" -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake\n"
      "    Linux/macOS:\n"
      "      cmake . -G \"Unix Makefiles\" -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake\n"
      "  2. Environment variable:\n"
      "    Windows:\n"
      "      set CMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake\n"
      "    Linux/macOS:\n"
      "      export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake\n"
      "Once you have installed raylib or set up vcpkg,\n"
      "remove CMake cache files and folders\n"
      "run `rm CMakeCache.txt ; rm -rf CMakeFiles/`\n"
      "and then try running CMake again.\n")
endif()

find_program(CLANG_FORMAT_EXE clang-format)

if(CLANG_FORMAT_EXE)
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_EXE} -style=Microsoft -i ${CMAKE_SOURCE_DIR}/main.cpp
    COMMENT "Running clang-format...")
  message("Code will be formatted using clang-format.")
else()
  add_custom_target(
    format
    COMMENT "Can't run clang-format...")
  message(WARNING "clang-format not found! Code formatting will be skipped.")
endif()

add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME} format)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
