CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( remu )

FIND_PACKAGE( SDL REQUIRED )

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} )

SET( CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=gnu11" )
SET( CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -Og -g" )
SET( CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -O3" )
SET( CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=gnu++11 -fno-rtti" )
SET( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Og -g" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" )

SET( SOURCES src/emulator.cpp
             src/memory.cpp
             src/vfp.cpp
             src/cpu.cpp
             src/nes.cpp
             src/bcm2835/gpio.cpp
             src/bcm2835/mbox.cpp
             src/bcm2835/framebuffer.cpp
             src/bcm2835/peripheral.cpp )

SET( LIBS m ${SDL_LIBRARY} )

INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/src" ${SDL_INCLUDE_DIRS} )

ADD_EXECUTABLE( remu src/main.cpp ${SOURCES} )

TARGET_LINK_LIBRARIES( remu ${LIBS} )
