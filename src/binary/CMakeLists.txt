INCLUDE_DIRECTORIES( inc )

INCLUDE_DIRECTORIES( ../oshal/inc )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )

ADD_LIBRARY( binary STATIC src/Binary.cpp
                           src/BinFile.cpp
                           src/ElfFile.cpp
                           src/NmSymbolFile.cpp
                           src/SymbolFile.cpp )

TARGET_LINK_LIBRARIES( binary oshal )

