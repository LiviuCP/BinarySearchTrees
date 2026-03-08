add_executable(SimpleBSTTests tst_simplebsttests.cpp ${TEST_UTILITIES_DIR}/testutils.cpp)
add_test(NAME SimpleBSTTests COMMAND SimpleBSTTests)

add_executable(RedBlackTreesTests tst_redblacktreestests.cpp ${TEST_UTILITIES_DIR}/testutils.cpp)
add_test(NAME RedBlackTreesTests COMMAND RedBlackTreesTests)

add_executable(AVLTreesTests tst_avltreestests.cpp ${TEST_UTILITIES_DIR}/testutils.cpp)
add_test(NAME AVLTreesTests COMMAND AVLTreesTests)

add_executable(MixedTreeTypesTests tst_mixedtreetypestests.cpp ${TEST_UTILITIES_DIR}/testutils.cpp)
add_test(NAME MixedTreeTypesTests COMMAND MixedTreeTypesTests)

target_compile_definitions(SimpleBSTTests PRIVATE PRINT_TREE)
target_compile_definitions(RedBlackTreesTests PRIVATE PRINT_TREE)
target_compile_definitions(AVLTreesTests PRIVATE PRINT_TREE)
target_compile_definitions(MixedTreeTypesTests PRIVATE PRINT_TREE)

target_link_libraries(SimpleBSTTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(SimpleBSTTests PRIVATE BSTLib)

target_link_libraries(RedBlackTreesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(RedBlackTreesTests PRIVATE BSTLib)

target_link_libraries(AVLTreesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(AVLTreesTests PRIVATE BSTLib)

target_link_libraries(MixedTreeTypesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(MixedTreeTypesTests PRIVATE BSTLib)
