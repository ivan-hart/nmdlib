cmake_minimum_required(VERSION 3.31.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(test VERSION "0.0.1" LANGUAGES C CXX)

# Include FetchContent module
include(FetchContent)

# Fetch and build SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

# Fetch and build assimp
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3
)
FetchContent_MakeAvailable(assimp)

# Fetch glm (header-only library)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm.git
)
FetchContent_MakeAvailable(glm)

# Source files
set(SOURCES
    src/main.cpp
    src/gl.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    assimp::assimp
    GL  # OpenGL is system-provided
)

# Include glm headers (header-only library)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Copy assets to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})