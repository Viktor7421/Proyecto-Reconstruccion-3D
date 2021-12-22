cmake_minimum_required(VERSION 3.20)
project(Proyecto3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proyecto3 glad.c stb_image.h stb_image.cpp filesystem.h main.cpp)

target_link_libraries(Proyecto3 -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3 -lGLEW32)