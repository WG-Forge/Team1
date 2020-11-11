cmake_minimum_required(VERSION 3.15)
project(Rail-simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

add_executable(Rail-simulator src/main.cpp src/Application.cpp src/Application.h src/Config.h src/Camera.cpp src/Camera.h src/Render.cpp src/Render.h src/State.h)

set(SFML_ROOT "lib/SFML-2.5.1")
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
set(SFML_INCLUDE_DIR "lib/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "lib/SFML-2.5.1/lib")

find_package(SFML 2.5.1 REQUIRED graphics network audio window system)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries (Rail-simulator sfml-graphics sfml-network sfml-audio sfml-window sfml-system)
endif()