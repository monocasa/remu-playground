INCLUDE_DIRECTORIES( inc )

ADD_LIBRARY( jitpp src/arm/Disassembler.cpp
                   src/arm/Dissector.cpp )

ADD_EXECUTABLE( test_jitpp test/arm/Test_Disassembler.cpp )

TARGET_LINK_LIBRARIES( test_jitpp jitpp gtest gtest_main )

ADD_TEST( NAME test_jitpp COMMAND "${EXECUTABLE_OUTPUT_PATH}/test_jitpp" )

