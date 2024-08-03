cmake_minimum_required(VERSION 3.17)

# Project/target name is passed from the main project to allow IDF to have a dependency on this target
# as well as embed the binary into the main app
project(${ULP_APP_NAME} LANGUAGES C CXX ASM)
add_executable(${ULP_APP_NAME} main.c)

# Import the ULP project helper functions
include(IDFULPProject)

# Apply default compile options
ulp_apply_default_options(${ULP_APP_NAME})

# Apply default sources provided by the IDF ULP component
ulp_apply_default_sources(${ULP_APP_NAME})

# Add targets for building the binary, as well as the linkerscript which exports ULP shared variables to the main app
ulp_add_build_binary_targets(${ULP_APP_NAME})

# Set custom compile flags
target_compile_options(${ULP_APP_NAME} PRIVATE -msave-restore)

set(CONFIG_RVFPLIB_SIZE ON)
include(cmake/ulp_tools.cmake)
ulp_print_size(${ULP_APP_NAME})
ulp_enable_rvfplib(${ULP_APP_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/RVfplib")
