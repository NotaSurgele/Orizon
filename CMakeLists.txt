cmake_minimum_required(VERSION 3.15)
project(Orizon)

set(CMAKE_USE_PTHREADS_INIT true)

# careful
set(CMAKE_BUILD_TYPE Release)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	message("Debug Mode")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4 -gdwarf-2")
else ()
	message("Release Mode")
endif ()

include(FetchContent)

if (MSVC)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /bigobj)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

message("Declaring SFML..")
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.x)

FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui
		GIT_TAG 35b1148efb839381b84de9290d9caf0b66ad7d03
)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
		imgui-sfml
		GIT_REPOSITORY https://github.com/SFML/imgui-sfml
		GIT_TAG 82dc2033e51b8323857c3ae1cf1f458b3a933c35
)

message(STATUS "Fetching ImGui-SFML...")

message("Downloading SFML...")
FetchContent_MakeAvailable(SFML)

message("Downloading IMGUI...")
FetchContent_MakeAvailable(IMGUI)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
FetchContent_MakeAvailable(imgui-sfml)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_definitions(-DDEBUG_MESSAGE=0)
add_definitions(-DENGINE_GUI)
file(GLOB_RECURSE SRC
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/Engine/
    ${CMAKE_SOURCE_DIR}/include/Engine/Scene/
	${CMAKE_SOURCE_DIR}/include/Engine/Components/
	${CMAKE_SOURCE_DIR}/include/Engine/Graphical/
    ${CMAKE_SOURCE_DIR}/include/external/
    ${CMAKE_SOURCE_DIR}/include/external/lua
    ${CMAKE_SOURCE_DIR}/include/Engine/Particle
    ${CMAKE_SOURCE_DIR}/include/Engine/Collision/
    ${CMAKE_SOURCE_DIR}/include/Engine/Collision/Layer
    ${CMAKE_SOURCE_DIR}/include/Engine/Scripting/
    ${CMAKE_SOURCE_DIR}/include/Engine/Scripting/sol
    ${CMAKE_SOURCE_DIR}/include/Game/
    ${SFML_SOURCE_DIR}/include/
)

# end
add_executable(${PROJECT_NAME} main.cpp ${SRC} ${LUA_FILES})

if (MSVC)
message("Visual studio C++ compilation")
add_compile_definitions(_USE_MATH_DEFINES)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /MP10)
target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
		ImGui-SFML::ImGui-SFML
	sfml-graphics
	sfml-window
	sfml-audio
	sfml-network
	sfml-system
	Threads::Threads
)
else()

target_link_libraries(
 	${PROJECT_NAME}
	PUBLIC
		ImGui-SFML::ImGui-SFML
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-network
    sfml-system
    pthread
)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
