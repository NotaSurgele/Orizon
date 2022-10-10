cmake_minimum_required(VERSION 3.15)
project(Orizon)

include(FetchContent)

#set(BUILD_SHARED_LIBS OFF)
message("Declaring SFML..")
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.x)

message("Downloading SFML...")
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
add_executable(
	${PROJECT_NAME}
	main.cpp
)
target_link_libraries(
 	${PROJECT_NAME}
	sfml-graphics
)
