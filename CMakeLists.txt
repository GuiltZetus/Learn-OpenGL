cmake_minimum_required(VERSION 3.10)

project(CreateWindow LANGUAGES C CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prefer modern OpenGL (GLVND)
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# Find system libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Build GLAD as a static library
add_library(glad src/glad.c)

# Tell CMake where glad headers are
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Your executable
add_executable(CreateWindow
    src/main.cpp
)

# Link everything together
target_link_libraries(CreateWindow
    glad
    glfw
    OpenGL::GL
)
