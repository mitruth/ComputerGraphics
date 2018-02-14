cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME
        "OpenGL_01"
        )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        GLEW/eglew.h
        GLEW/glew.h
        GLEW/glxew.h
        GLEW/wglew.h
        GLFW/glfw3.h
        GLFW/glfw3native.h
        OpenGL_4_Application_VS2015.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

