cmake_minimum_required(VERSION 3.15)

project(CreateWindow LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------
# C++ standard
# --------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --------------------
# Executable
# --------------------
add_executable(CreateWindow
    src/main.cpp
    src/engine/Shader.cpp
    src/engine/VBO.cpp
    src/engine/VAO.cpp
    src/engine/Camera.cpp
    src/tools/stb_image.cpp
)

# --------------------
# Include directories
# --------------------
target_include_directories(CreateWindow PRIVATE
    include                # include/engine/Shader.h
    external/stb           # stb_image.h
    external/glad/include  # glad.h
)

# --------------------
# GLAD (static library)
# --------------------
add_library(glad STATIC
    external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# --------------------
# OpenGL & GLFW
# --------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# --------------------
# Link libraries
# --------------------
target_link_libraries(CreateWindow PRIVATE
    glad
    OpenGL::GL
    glfw
)

# --------------------
# Assets (shaders)
# --------------------
# Copy shaders to build folder so relative paths work
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
