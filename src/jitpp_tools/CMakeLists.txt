INCLUDE_DIRECTORIES( ../jitpp/inc )

ADD_EXECUTABLE( jitpp-dis dis.cpp )

TARGET_LINK_LIBRARIES( jitpp-dis jitpp )

