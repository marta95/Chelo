cmake_minimum_required(VERSION 3.5)
project(SFMLtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(Sources)

set(SOURCE_FILES Sources/main.cpp Sources/Source.h.h Sources/AST.h Sources/Parser.h Sources/optional.h)
add_executable(SFMLtest ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(OpenGL REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(SFMLtest ${SFML_LIBRARIES})
endif()

if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(SFMLtest ${OPENGL_LIBRARIES})
endif()