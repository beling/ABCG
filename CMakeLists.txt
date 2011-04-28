cmake_minimum_required (VERSION 2.6)
project (ABCGame)

#include_directories (${ABCG_SOURCE_DIR}/libs)

file(GLOB sources_files *.cpp libs/*.cpp *.h libs/*.h)

# add library with name libname to list liblist, include directory with this library
# extra arguments are pass to find_package
#MACRO(addlib liblist libname)
#	find_package(${libname} REQUIRED ${ARGN})
#	include_directories(${${libname}_INCLUDE_DIRS}})
#	set(${liblist} ${${liblist}} ${${libname}_LIBRARIES})
#ENDMACRO(addlib libname liblist)
#addlib(LIBS GLUT)
#addlib(LIBS OpenGL)

# add libarary with name libname to list liblist
# extra arguments are pass to find_library
MACRO(findlib liblist libname)
    find_library(${libname}_LIB ${libname} PATHS ${ARGN} ${CMAKE_SOURCE_DIR}/extlib/lib)
    if(NOT ${libname}_LIB)
        message(FATAL_ERROR "Unable to find library: " ${libname})
    endif(NOT ${libname}_LIB)
    message(${libname} " found at: " ${${libname}_LIB})
    set(${liblist} ${${liblist}} ${${libname}_LIB})
ENDMACRO(findlib liblist libname)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLUT_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OPENAL_LIBRARY})

findlib(LIBS alure)

add_executable(ABCG ${sources_files})
target_link_libraries(ABCG ${LIBS})

#SET_TARGET_PROPERTIES(ABCG PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
