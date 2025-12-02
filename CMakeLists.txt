cmake_minimum_required(VERSION 3.21)

project(jankeytype VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# STAGE 1: Check System Raylib
# -----------------------------------------------------------------------------
message(STATUS "STAGE 1: Attempting to find system Raylib...")
find_package(raylib QUIET)
find_package(glfw3 CONFIG QUIET)

if(raylib_FOUND)
    message(STATUS "Success! Found system Raylib.")
else()
    message(STATUS "System Raylib not found. Proceeding to checking vcpkg...")

    # -------------------------------------------------------------------------
    # STAGE 2: Check for Existing Vcpkg (Environment Variable)
    # -------------------------------------------------------------------------
    if(DEFINED ENV{VCPKG_ROOT})
        message(STATUS "STAGE 2: Found VCPKG_ROOT at $ENV{VCPKG_ROOT}")
        set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
        set(VCPKG_EXE "$ENV{VCPKG_ROOT}/vcpkg")
        
        # Windows usually has vcpkg.exe
        if(WIN32 AND NOT EXISTS "${VCPKG_EXE}")
            set(VCPKG_EXE "${VCPKG_EXE}.exe")
        endif()
        
    # -------------------------------------------------------------------------
    # STAGE 3: Local Fallback (Clone Vcpkg)
    # -------------------------------------------------------------------------
    else()
        message(STATUS "STAGE 3: No global vcpkg found. Cloning local vcpkg...")
        set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/vcpkg")
        
        # 3a. Clone if missing
        if(NOT EXISTS "${VCPKG_ROOT}")
            execute_process(
                COMMAND git clone --depth 1 https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}"
                RESULT_VARIABLE _vcpkg_git_clone_rc
                OUTPUT_QUIET ERROR_QUIET
            )
            if(NOT _vcpkg_git_clone_rc EQUAL 0)
                message(FATAL_ERROR "Failed to clone vcpkg. Ensure git is installed.")
            endif()
        endif()

        # 3b. Bootstrap
        if(WIN32)
            set(VCPKG_EXE "${VCPKG_ROOT}/vcpkg.exe")
            if(NOT EXISTS "${VCPKG_EXE}")
                message(STATUS "Bootstrapping local vcpkg (Windows)...")
                execute_process(COMMAND cmd /c bootstrap-vcpkg.bat WORKING_DIRECTORY "${VCPKG_ROOT}" OUTPUT_QUIET ERROR_QUIET)
            endif()
        else()
            set(VCPKG_EXE "${VCPKG_ROOT}/vcpkg")
            if(NOT EXISTS "${VCPKG_EXE}")
                message(STATUS "Bootstrapping local vcpkg (Unix)...")
                execute_process(COMMAND bash -c "./bootstrap-vcpkg.sh" WORKING_DIRECTORY "${VCPKG_ROOT}" OUTPUT_QUIET ERROR_QUIET)
            endif()
        endif()
    endif()

    # -------------------------------------------------------------------------
    # INSTALLATION & LINKING (Common for Stage 2 & 3)
    # -------------------------------------------------------------------------
    
    # 1. Determine Triplet (Architecture)
    if(WIN32)
        set(VCPKG_DEFAULT_TRIPLET "x64-windows")
    elseif(APPLE)
        if(CMAKE_OSX_ARCHITECTURES MATCHES "(arm64|aarch64)")
            set(VCPKG_DEFAULT_TRIPLET "arm64-osx")
        else()
            set(VCPKG_DEFAULT_TRIPLET "x64-osx")
        endif()
    else()
        set(VCPKG_DEFAULT_TRIPLET "x64-linux")
    endif()

    # 2. Run Install (Uses vcpkg.json)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json")
        message(STATUS "Using vcpkg at: ${VCPKG_EXE}")
        message(STATUS "Installing dependencies for triplet: ${VCPKG_DEFAULT_TRIPLET}...")
        
        execute_process(
            COMMAND "${VCPKG_EXE}" install --triplet "${VCPKG_DEFAULT_TRIPLET}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            RESULT_VARIABLE _vcpkg_install_rc
        )
        if(NOT _vcpkg_install_rc EQUAL 0)
            message(FATAL_ERROR "vcpkg install failed.")
        endif()
    else()
        message(WARNING "No vcpkg.json found! Dependency installation might fail.")
    endif()

    # 3. Tell CMake where to find the libraries vcpkg just built
    # Manifest mode (vcpkg.json) installs to specific locations relative to the root
    set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/${VCPKG_DEFAULT_TRIPLET}")

    # Append these paths so find_package looks here
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}/share")

    # 4. Try finding Raylib again
    find_package(raylib CONFIG REQUIRED)
    find_package(glfw3 CONFIG QUIET)
endif()

# -----------------------------------------------------------------------------
# CODE FORMATTING & BUILD
# -----------------------------------------------------------------------------
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT_EXE} -style=Microsoft -i "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        COMMENT "Running clang-format..."
    )
endif()

add_executable(${PROJECT_NAME} main.cpp)

if(CLANG_FORMAT_EXE)
    add_dependencies(${PROJECT_NAME} format)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()
