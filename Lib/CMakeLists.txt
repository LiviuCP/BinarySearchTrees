project(BSTLib LANGUAGES CXX)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    binarysearchtreeslib.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET bst_modules
        TYPE CXX_MODULES
        FILES
            binarysearchtree.cpp
            binarysearchtree_header.cpp
            binarysearchtree_impl.cpp
            redblacktree.cpp
            redblacktree_header.cpp
            redblacktree_impl.cpp
            avltree.cpp
            avltree_header.cpp
            avltree_impl.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE BINARYSEARCHTREESLIB_LIBRARY)
