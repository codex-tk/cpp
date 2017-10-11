project( codex.tests )

include_directories( ${CMAKE_SOURCE_DIR}/codex/include )

file(GLOB SRCS ./*.cpp)

add_executable( codex.tests ${SRCS} )
target_link_libraries( codex.tests codex gtest )

enable_testing()
add_test(codex.tests codex.tests)

add_custom_command(TARGET codex.tests
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --output-on-failure)