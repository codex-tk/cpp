project( codex.tests )

include_directories( ${CMAKE_SOURCE_DIR}/codex/include )
include_directories( ${CMAKE_SOURCE_DIR}/json/src )

file(GLOB_RECURSE SRCS "*.cpp" )
#file(GLOB SRCS ./*.cpp)

add_executable( codex.tests ${SRCS} )

find_package( ZLIB )
if ( ZLIB_FOUND )
    target_link_libraries( codex.tests ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

if(Boost_FOUND)
    target_link_libraries( codex.tests ${Boost_LIBRARIES})
endif()

target_link_libraries( codex.tests codex gtest  )

enable_testing()
add_test(codex.tests codex.tests)
add_custom_command(TARGET codex.tests
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --verbose
    #--output-on-failure
    )