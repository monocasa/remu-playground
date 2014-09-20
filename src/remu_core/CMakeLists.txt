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
                       src/host/linux/mem.cpp
                       src/emulator.cpp
                       src/gpioblock.cpp
                       src/keydispatcher.cpp
                       src/memory.cpp )

TARGET_LINK_LIBRARIES( remu_core ${SDL_LIBRARY} )

