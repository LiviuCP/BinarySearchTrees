project(BSTLib LANGUAGES CXX)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    binarysearchtreeslib.cpp
    binarysearchtree.cpp
    redblacktree.cpp
    avltree.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE BINARYSEARCHTREESLIB_LIBRARY)
