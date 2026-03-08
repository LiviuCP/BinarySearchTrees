project(BSTApp LANGUAGES CXX)

add_executable(${PROJECT_NAME} binarysearchtreesappmain.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TREE)

target_link_libraries(${PROJECT_NAME} PRIVATE BSTLib)
