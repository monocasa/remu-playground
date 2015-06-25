IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
	SET( OSHAL_SOURCES ${OSHAL_SOURCES}
                       src/linux/Mem.cpp )
ELSE()
	MESSAGE( FATAL_ERROR "Unknown OS in liboshal" )
ENDIF()

INCLUDE_DIRECTORIES( inc )
INCLUDE_DIRECTORIES( ../util/inc )

ADD_LIBRARY( oshal STATIC ${OSHAL_SOURCES} )

