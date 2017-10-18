project( codex.tests )

include_directories( ${CMAKE_SOURCE_DIR}/codex/include )

file(GLOB SRCS ./*.cpp)

add_executable( codex.tests ${SRCS} )

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( codex.tests ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

target_link_libraries( codex.tests codex gtest  )

enable_testing()
add_test(codex.tests codex.tests)
add_custom_command(TARGET codex.tests
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --verbose
    #--output-on-failure
    )