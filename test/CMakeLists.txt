ADD_EXECUTABLE( test_remu_core Test_IoRegion.cpp )

TARGET_LINK_LIBRARIES( test_remu_core remu_core gtest gtest_main )

ADD_TEST( NAME test_remu_core COMMAND "${EXECUTABLE_OUTPUT_PATH}/test_remu_core" )

