INCLUDE_DIRECTORIES( ../oshal/inc )

ADD_LIBRARY( remu_core src/arm/Cpu_core.cpp
                       src/arm/Cpu_vfp.cpp
                       src/bcm2835/Framebuffer.cpp
                       src/bcm2835/Gpio.cpp
                       src/bcm2835/Mbox.cpp
                       src/bcm2835/Peripheral.cpp
                       src/bcm2835/RPiEmulator.cpp
                       src/bcm2835/Timer.cpp
                       src/bitbang/Button.cpp
                       src/bitbang/Nes.cpp
                       src/kvm/CrossVmm.cpp
                       src/kvm/KvmContext.cpp
                       src/loader/LoadElf.cpp
                       src/Emulator.cpp
                       src/GpioBlock.cpp
                       src/KeyDispatcher.cpp
                       src/Memory.cpp )

TARGET_LINK_LIBRARIES( remu_core oshal ${SDL_LIBRARY} )

ADD_SUBDIRECTORY( test )

