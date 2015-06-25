INCLUDE_DIRECTORIES( inc )

ADD_LIBRARY( binary STATIC src/Binary.cpp
                           src/BinFile.cpp
                           src/ElfFile.cpp )

