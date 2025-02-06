cmake_minimum_required(VERSION 3.20)
project(PatienceGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Special handling for macOS
if(APPLE)
    # Add Homebrew paths for ARM64 macOS
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(HOMEBREW_PREFIX "/opt/homebrew")
    else()
        set(HOMEBREW_PREFIX "/usr/local")
    endif()
    
    list(APPEND CMAKE_PREFIX_PATH ${HOMEBREW_PREFIX})
    list(APPEND CMAKE_LIBRARY_PATH ${HOMEBREW_PREFIX}/lib)
    list(APPEND CMAKE_INCLUDE_PATH ${HOMEBREW_PREFIX}/include)
endif()

# Find pkg-config
find_package(PkgConfig REQUIRED)

# Use pkg-config to find Cairo
pkg_check_modules(CAIRO REQUIRED cairo)

# Extract Cairo library path from pkg-config
execute_process(
    COMMAND pkg-config --libs-only-L cairo
    OUTPUT_VARIABLE CAIRO_LIBRARY_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Convert -L flags to actual paths
string(REPLACE "-L" "" CAIRO_LIBRARY_DIRS "${CAIRO_LIBRARY_DIRS}")
string(REPLACE " " ";" CAIRO_LIBRARY_DIRS "${CAIRO_LIBRARY_DIRS}")

# Find the actual Cairo library
find_library(CAIRO_LIBRARY
    NAMES cairo
    PATHS ${CAIRO_LIBRARY_DIRS}
    REQUIRED
)

# Find X11
find_package(X11)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CAIRO_INCLUDE_DIRS}
)

# Create the main executable
add_executable(patience_game src/main.cpp)

# Link directories
link_directories(${CAIRO_LIBRARY_DIRS})

# Link libraries with explicit Cairo library path
target_link_libraries(patience_game
    PRIVATE
    patience_core
    ${CAIRO_LIBRARY}
)

# Add link directory hints for macOS
if(APPLE)
    set_target_properties(patience_game PROPERTIES
        LINK_FLAGS "-L${CAIRO_LIBRARY_DIRS}"
    )
endif()

# Enable testing
enable_testing()