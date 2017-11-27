project( codex )
include_directories( ${CMAKE_SOURCE_DIR}/codex/include )

file(GLOB_RECURSE HEADERS RELATIVE "${CMAKE_SOURCE_DIR}/codex" "include/*.hpp" )
file(GLOB_RECURSE SRCS RELATIVE "${CMAKE_SOURCE_DIR}/codex" "src/*.cpp" )

add_library( codex STATIC ${HEADERS} ${SRCS} )
#add_library( codex SHARED ${HEADERS} ${SRCS} )
