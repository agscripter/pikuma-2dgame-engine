cmake_minimum_required(VERSION 3.22)
project(pikuma_2dgame_engine)

set(CMAKE_CXX_STANDARD 17)

# --- Variables
set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SOURCE_FILES src/main.cpp src/game.cpp src/game.h)

# --- Dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
        "src/"
        ${SDL2_INCLUDE}
        ${SDL2_TTF_INCLUDE}
        ${SDL2_IMAGE_INCLUDE}
)

# --- Resources
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# --- Executable
add_executable(pikuma_2dgame_engine ${SOURCE_FILES})

# --- Including dependencies
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
)