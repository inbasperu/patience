# Enable testing for this directory
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)

# Create test executable
add_executable(patience_tests
    core/card_test.cpp
)

# Link against Google Test and our library
target_link_libraries(patience_tests
    PRIVATE
    GTest::GTest
    GTest::Main
    patience_core
)

# Add test to CTest
add_test(NAME card_tests COMMAND patience_tests)

# Set include directories
target_include_directories(patience_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)