SET( CMAKE_CXX_FLAGS "-m64 -mcmodel=kernel -std=c++1y -D__64BIT__ -I${PROJECT_SOURCE_DIR}/libjitpp/include -I${PROJECT_SOURCE_DIR}/libutil/include -I${CMAKE_CURRENT_LIST_DIR}/stdlib/include -I${CMAKE_CURRENT_LIST_DIR}/stdlib/newlib/libc/include -ffreestanding -nostdlib -mno-red-zone -fno-rtti -fno-exceptions -nostdinc -fno-stack-protector -Wall -Wextra -Werror" )
SET( CMAKE_C_FLAGS "-m64 -mcmodel=kernel -D__64BIT__  -I${CMAKE_CURRENT_LIST_DIR}/stdlib/include -I${CMAKE_CURRENT_LIST_DIR}/stdlib/newlib/libc/include -ffreestanding -nostdlib -mno-red-zone -nostdinc -fno-stack-protector -Wall -Wextra -Werror" )
SET( CMAKE_EXE_LINKER_FLAGS "-nodefaultlibs -T${CMAKE_CURRENT_LIST_DIR}/linker.ld" )
SET( CMAKE_ASM_NASM_OBJECT_FORMAT "elf64" )

ENABLE_LANGUAGE( ASM_NASM )

ADD_LIBRARY( vmm_os STATIC src/os/Boot.asm
                           src/os/Init.cpp )

ADD_LIBRARY( vmm_app STATIC src/app/Main.cpp )

SET( VMMLIBS "${LIBRARY_OUTPUT_PATH}/libvmm_os.a" 
             "${LIBRARY_OUTPUT_PATH}/libvmm_app.a" )

SET( LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/Linker.ld" )

ADD_CUSTOM_COMMAND( OUTPUT "${EXECUTABLE_OUTPUT_PATH}/vmm.bin"
                    COMMAND ld -nodefaultlibs -T${LINKER_SCRIPT} -o "${EXECUTABLE_OUTPUT_PATH}/vmm.bin" ${VMMLIBS}
                    DEPENDS vmm_os vmm_app ${LINKER_SCRIPT} )

ADD_CUSTOM_TARGET( vmm
                   ALL
                   DEPENDS "${EXECUTABLE_OUTPUT_PATH}/vmm.bin" )

