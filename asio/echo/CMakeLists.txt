project( asio.echo )

file(GLOB_RECURSE SRCS "*.cpp" )
add_executable( asio.echo ${SRCS} )

if(Boost_FOUND)
    target_link_libraries( asio.echo ${Boost_LIBRARIES})
endif()