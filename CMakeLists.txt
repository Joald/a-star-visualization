cmake_minimum_required(VERSION 3.13)
project(a_star_visualization)

set(CMAKE_CXX_STANDARD 17)

add_executable(a_star_visualization main.cpp Menu.cpp Menu.h Window.cpp Window.h FieldSelector.cpp FieldSelector.h)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/../The-Station/")
set(SFML_INCLUDE_DIR SFML/include)
set(SFML_DIR SFML/cmake-build-debug)
set(SFML_LIBRARIES sfml-graphics sfml-network sfml-audio)
find_package(SFML 2.5 REQUIRED system window graphics network audio)
if (SFML_FOUND)
    message("SFML was found!")
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(a_star_visualization ${SFML_LIBRARIES})
endif ()