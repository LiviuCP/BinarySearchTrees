cmake_minimum_required(VERSION 3.28)

project(BST LANGUAGES CXX)

add_compile_definitions(PRINT_TREE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include_directories(Lib)

add_subdirectory(App)
add_subdirectory(Lib)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test QUIET)

if (QT_FOUND)
    add_subdirectory(Tests)
endif()
