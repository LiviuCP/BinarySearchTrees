project(BSTTests LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

SET(TEST_UTILITIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Utilities)

include_directories(${TEST_UTILITIES_DIR})

add_subdirectory(IntKeyBSTTests)
add_subdirectory(UtilitiesTests)
