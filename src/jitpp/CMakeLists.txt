INCLUDE_DIRECTORIES( inc )

SET( JITPP_SOURCES src/arm/ArmTranslator.cpp
                   src/arm/Disassembler.cpp
                   src/arm/Dissector.cpp
                   src/ppc/Disassembler.cpp
                   src/ppc/Dissector.cpp
                   src/BasicBlockInterpreter.cpp )

ADD_LIBRARY( jitpp STATIC ${JITPP_SOURCES} )

ADD_EXECUTABLE( test_jitpp test/arm/Test_Disassembler.cpp )

TARGET_LINK_LIBRARIES( test_jitpp jitpp gtest gtest_main ${CMAKE_THREAD_LIBS_INIT} )

ADD_TEST( NAME test_jitpp COMMAND "${EXECUTABLE_OUTPUT_PATH}/test_jitpp" )

ADD_LIBRARY( jitpp_vmm STATIC ${JITPP_SOURCES} )

SET_TARGET_PROPERTIES( jitpp_vmm PROPERTIES 
                                 COMPILE_FLAGS "-m64 -mcmodel=kernel -std=c++1y -D__64BIT__ -I${PROJECT_SOURCE_DIR}/src/jitpp/inc -I${PROJECT_SOURCE_DIR}/src/vmm/inc -ffreestanding -nostdlib -mno-red-zone -fno-rtti -fno-exceptions -nostdinc -fno-stack-protector -Wall -Wextra -Werror" )

