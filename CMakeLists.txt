CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( remu )

FIND_PACKAGE( SDL REQUIRED )

SET( LIBARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )

SET( CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=gnu11" )
SET( CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -Og -g" )
SET( CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -O3" )
SET( CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=gnu++11 -fno-rtti" )
SET( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Og -g" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" )

INCLUDE_DIRECTORIES( SYSTEM
                     "${PROJECT_SOURCE_DIR}/3rdparty/google-test/include" )

ADD_SUBDIRECTORY( 3rdparty/google-test )

ENABLE_TESTING()

INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/src" ${SDL_INCLUDE_DIRS} )

ADD_LIBRARY( remu_core src/arm/cpu_core.cpp
                       src/arm/cpu_vfp.cpp
                       src/bcm2835/framebuffer.cpp
                       src/bcm2835/gpio.cpp
                       src/bcm2835/mbox.cpp
                       src/bcm2835/peripheral.cpp
                       src/bcm2835/rpiemulator.cpp
                       src/bcm2835/timer.cpp
                       src/bitbang/button.cpp
                       src/bitbang/nes.cpp
                       src/emulator.cpp
                       src/gpioblock.cpp
                       src/keydispatcher.cpp
                       src/memory.cpp )

TARGET_LINK_LIBRARIES( remu_core ${SDL_LIBRARY} )

ADD_EXECUTABLE( remu src/main.cpp )

TARGET_LINK_LIBRARIES( remu m remu_core  )

ADD_SUBDIRECTORY( test )

