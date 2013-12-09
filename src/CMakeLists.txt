project (MultiCurses)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
FIND_PACKAGE(NCURSES REQUIRED)
INCLUDE_DIRECTORIES(${NCURSES_INCLUDE_DIR} ${pthread_INCLUDE_DIR})
cmake_minimum_required(VERSION 2.8) # http://www.cmake.org/cmake/help/cmake-2-8-docs.html#command:cmake_minimum_required

add_definitions(--std=c++11)

add_executable(MultiCurses CursesWindow.cpp CursesListWindow.cpp  MultiCurses.cpp)
TARGET_LINK_LIBRARIES(MultiCurses  ${NCURSES_LIBRARY} gcov pthread )
