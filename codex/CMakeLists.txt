project( codex )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
#[[
if ( MSVC )
	#set(BOOST_ROOT "C:/Users/codex/works/boost_1_64_0")
	#set(BOOST_LIBRARYDIR "C:/Users/codex/works/boost_1_64_0/stage/lib")
    set( PLATFORM_DEPEND_HEADERS

    )
    set( PLATFORM_DEPEND_SRCS

    )
else()
  if ( APPLE )
    set( PLATFORM_DEPEND_HEADERS

    )
    set( PLATFORM_DEPEND_SRCS

    )
  else()
    set( PLATFORM_DEPEND_HEADERS
    )
    set( PLATFORM_DEPEND_SRCS
    )
  endif()
#common
  set( PLATFORM_DEPEND_HEADERS
      ${PLATFORM_DEPEND_HEADERS}
  )
  set( PLATFORM_DEPEND_SRCS
      ${PLATFORM_DEPEND_SRCS}
  )
endif()

#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME ON)

#find_package (Boost 1.60.0 REQUIRED COMPONENTS system date_time regex )

#if (Boost_FOUND)
#  include_directories (SYSTEM ${Boost_INCLUDE_DIR})
#endif()

set( HEADERS
    include/codex/codex.hpp
    ${PLATFORM_DEPEND_HEADERS}
    )

set( SRCS
    src/codex.cpp
    ${PLATFORM_DEPEND_SRCS}
    )
]]

file(GLOB_RECURSE HEADERS RELATIVE "${CMAKE_SOURCE_DIR}/codex" "include/*.hpp" )
file(GLOB_RECURSE SRCS RELATIVE "${CMAKE_SOURCE_DIR}/codex" "src/*.cpp" )

add_library( codex STATIC ${HEADERS} ${SRCS} )
#add_library( codex SHARED ${HEADERS} ${SRCS} )
