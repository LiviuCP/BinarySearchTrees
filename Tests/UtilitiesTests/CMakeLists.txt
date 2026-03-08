add_executable(StringifiedIntegerTests tst_stringifiedintegertests.cpp ${TEST_UTILITIES_DIR}/stringifiedinteger.cpp)
add_test(NAME StringifiedIntegerTests COMMAND StringifiedIntegerTests)

target_link_libraries(StringifiedIntegerTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
