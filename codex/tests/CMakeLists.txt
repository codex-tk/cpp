project( codex.tests )

include_directories( ${CMAKE_SOURCE_DIR}/codex/include )
include_directories( ${CMAKE_SOURCE_DIR}/json/src )

file(GLOB SRCS ./*.cpp)

add_executable( codex.tests ${SRCS} )

find_package( ZLIB )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( codex.tests ${ZLIB_LIBRARIES} )
    add_definitions( -DENABLE_LIB_Z )
endif( ZLIB_FOUND )

find_package(Boost 1.60.0 COMPONENTS filesystem )
if(Boost_FOUND)
    set(Boost_USE_STATIC_LIBS        ON) # only find static libs
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries( codex.tests ${Boost_LIBRARIES})
    add_definitions( -DENABLE_LIB_BOOST )
endif()

target_link_libraries( codex.tests codex gtest  )

enable_testing()
add_test(codex.tests codex.tests)
add_custom_command(TARGET codex.tests
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --verbose
    #--output-on-failure
    )