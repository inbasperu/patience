# Create the core library
add_library(patience_core
    core/card.cpp
)

# Set include directories for the library
target_include_directories(patience_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Set compiler features
target_compile_features(patience_core
    PUBLIC
    cxx_std_17
)