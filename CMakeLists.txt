cmake_minimum_required (VERSION 2.6)
project (ABCGame)

#include_directories (${ABCG_SOURCE_DIR}/libs)

file(GLOB sources_files *.cpp libs/*.cpp *.h libs/*.h)

# add library with name libname to list liblist, include directory with this library
# extra arguments are pass to find_package
MACRO(addlib liblist libname)
	find_package(${libname} REQUIRED ${ARGN})
	include_directories(${${libname}_INCLUDE_DIRS}})
	set(${liblist} ${${liblist}} ${${libname}_LIBRARIES})
ENDMACRO(addlib libname liblist)

addlib(LIBS GLUT)
addlib(LIBS OpenGL)

#find_package(GLUT REQUIRED)
#include_directories(${GLUT_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${GLUT_LIBRARIES})

#find_package(OpenGL REQUIRED)
#include_directories(${OpenGL_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${OpenGL_LIBRARIES})

add_executable(ABCG ${sources_files})
target_link_libraries(ABCG ${LIBS})

#SET_TARGET_PROPERTIES(ABCG PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
