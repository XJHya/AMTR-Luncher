cmake_minimum_required(VERSION 3.14)
project(runner LANGUAGES CXX)

# Set the target platform to Windows
if (WIN32)
  set(FLUTTER_TARGET_PLATFORM "windows")
endif()

# Define the application target
add_executable(${BINARY_NAME} WIN32
  "flutter_window.cpp"
  "main.cpp"
  "utils.cpp"
  "win32_window.cpp"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "Runner.rc"
  "runner.exe.manifest"
)

# Apply standard build settings
apply_standard_settings(${BINARY_NAME})

# Add preprocessor definitions for Flutter version
target_compile_definitions(${BINARY_NAME} PRIVATE
  "FLUTTER_VERSION=\"${FLUTTER_VERSION}\""
  "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}"
  "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}"
  "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}"
  "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}"
)

# Disable conflicting Windows macros
target_compile_definitions(${BINARY_NAME} PRIVATE "NOMINMAX")

# Link to Flutter libraries
target_link_libraries(${BINARY_NAME} PRIVATE
  flutter
  flutter_wrapper_app
  "dwmapi.lib"
)

# Include directories
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

# Add Flutter build dependencies
add_dependencies(${BINARY_NAME} flutter_assemble)
