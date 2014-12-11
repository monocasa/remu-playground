ADD_EXECUTABLE( test_util Test_bitops.cpp )

TARGET_LINK_LIBRARIES( test_util gtest gtest_main )

ADD_TEST( NAME test_util COMMAND "${EXECUTABLE_OUTPUT_PATH}/test_util" )

