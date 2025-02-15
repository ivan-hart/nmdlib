cmake_minimum_required(VERSION 3.31.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(test VERSION "0.0.1" LANGUAGES C CXX)

# Include FetchContent module for potential dependency fetching
include(FetchContent)

# Try to find system-installed SDL3
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found - fetching from repository")
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(SDL3)
endif()

# Try to find system-installed assimp (v5.4.3)
find_package(assimp 5.4.3 QUIET)
if(NOT assimp_FOUND)
    message(STATUS "assimp 5.4.3 not found - fetching from repository")
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.4.3
    )
    FetchContent_MakeAvailable(assimp)
endif()

# Try to find system-installed glm
find_package(glm QUIET)
if(NOT glm_FOUND)
    message(STATUS "glm not found - fetching from repository")
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/icaven/glm.git
    )
    FetchContent_MakeAvailable(glm)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/gl.c
    src/nmdlib/singleton.cpp
    src/nmdlib/shader.cpp
    src/nmdlib/mesh.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    assimp::assimp
    GL  # OpenGL is system-provided
)

# Handle glm dependencies
if(glm_FOUND)
    # Use system-installed glm through its imported target
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
else()
    # Use fetched glm by including its source directory
    target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
endif()

# Include project's own headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Copy assets to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})