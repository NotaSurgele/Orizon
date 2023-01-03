cmake_minimum_required(VERSION 3.15)
project(Orizon)

set(CMAKE_BUILD_TYPE Debug)
include(FetchContent)

message("Declaring SFML..")
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.x)

message("Downloading SFML...")
FetchContent_MakeAvailable(SFML)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_definitions(-DDEBUG_MESSAGE=0)
file(GLOB_RECURSE SRC
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/Engine/
    ${CMAKE_SOURCE_DIR}/include/Engine/Scene/
    ${CMAKE_SOURCE_DIR}/include/Engine/Components/
    # ${CMAKE_SOURCE_DIR}/include/Game/CustomComponents
    ${SFML_SOURCE_DIR}/include/
)

add_executable(${PROJECT_NAME} main.cpp ${SRC})

target_link_libraries(
 	${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-network
    sfml-system
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
