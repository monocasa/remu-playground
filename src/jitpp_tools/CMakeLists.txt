INCLUDE_DIRECTORIES( ../binary/inc )
INCLUDE_DIRECTORIES( ../jitpp/inc )
INCLUDE_DIRECTORIES( ../oshal/inc )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )

ADD_EXECUTABLE( jitpp-dis dis.cpp )

TARGET_LINK_LIBRARIES( jitpp-dis binary jitpp )

