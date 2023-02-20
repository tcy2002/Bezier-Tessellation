cmake_minimum_required(VERSION 3.22)
project(hw2)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ..)
set(DCMAKE_BUILD_TYPE Release)

link_directories(env/lib/x64)
include_directories(env/include)
include_directories(.)

add_executable(
        hw2
        main.cpp
        config/config.h
        shader/shader.h
        shader/shaderBuf.h
        bezier/bezier.h
        model/model.h
        view/view.h
        texture/texture.h
        bezier/bezier.cpp
        model/model.cpp
        view/view.cpp config/config.cpp sourcesLoader/sourcesLoader.cpp sourcesLoader/sourcesLoader.h shader/shader.cpp texture/texture.cpp)

target_link_libraries(
        hw2
        glew32.lib
        libfreeglut.a
        libopengl32.a)
