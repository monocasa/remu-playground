INCLUDE_DIRECTORIES( ../oshal/inc )

ADD_EXECUTABLE( remu main.cpp )

TARGET_LINK_LIBRARIES( remu m remu_core  )

