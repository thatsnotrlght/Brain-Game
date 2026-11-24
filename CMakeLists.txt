cmake_minimum_required(VERSION 3.21)
project(CAP4052_HW2_TransformingSnake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(CAP4052_HW2
    src/main.cpp
    src/Scene.cpp
    src/Renderer3D.cpp
    src/Model.cpp

)
target_compile_definitions(CAP4052_HW2 PRIVATE
    BRAIN_MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/models/Brain.obj"
    VIRUS_MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/models/virus.obj"
    NEURON_MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/models/neuron.obj"
)
target_include_directories(CAP4052_HW2 PRIVATE 
src
external/tinyobjloader
)
target_link_libraries(CAP4052_HW2 PRIVATE
    glfw
    glad::glad
    glm::glm
)
if (MSVC)
    target_compile_options(CAP4052_HW2 PRIVATE /W4 /permissive-)
else()
    target_compile_options(CAP4052_HW2 PRIVATE -Wall -Wextra -Wpedantic)
endif()
