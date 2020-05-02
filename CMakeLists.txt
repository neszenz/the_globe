CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(the_globe)

SET(source_dir "${PROJECT_SOURCE_DIR}/src/")
SET(exec_name "the_globe")

FILE(GLOB source_files "${source_dir}/*.cpp")

# +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
SET(OpenGL_GL_PREFERENCE "GLVND")
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp HINTS /usr/include /usr/local/include /opt/local/include)

# compiler settings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -std=c++17")

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GLM_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/inc)

LINK_LIBRARIES(${GLEW_LIBRARIES})
LINK_LIBRARIES(${OPENGL_LIBRARIES})
LINK_LIBRARIES(glfw)

ADD_EXECUTABLE(${exec_name} ${source_files})
