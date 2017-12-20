CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

project( cpp )

#option( MyOption "MyOption" OFF )
#cmake -DMyOption=ON project -D...

if(MSVC)
  # vs option chage 
  add_compile_options("/wd4819")
  add_compile_options("/wd4127")

  # for googletest
  foreach (flag_var
      CMAKE_CXX_FLAGS
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
    string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
  endforeach()

  string(REGEX REPLACE "/O2" "Od"
    CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG}")

  add_definitions( /std:c++latest )
  add_definitions( -DGTEST_LANG_CXX11=1 )
else()
  #-std=c++1z 
  add_definitions( -Wall -std=c++17 -o2 -g -ggdb -fconcepts )
endif()

find_package( ZLIB )
if ( ZLIB_FOUND )
  include_directories( ${ZLIB_INCLUDE_DIRS} )
  #target_link_libraries( codex.tests ${ZLIB_LIBRARIES} )
  add_definitions( -DENABLE_LIB_Z )
endif( ZLIB_FOUND )


#set(Boost_NO_SYSTEM_PATHS TRUE) 
#if (Boost_NO_SYSTEM_PATHS)
#  set(BOOST_ROOT "/Users/ghtak/boost")
#  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
#  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
#endif (Boost_NO_SYSTEM_PATHS)

find_package(Boost 1.60.0 COMPONENTS filesystem )
if(Boost_FOUND)
	message(STATUS  ${Boost_INCLUDE_DIRS} )	
    set(Boost_USE_STATIC_LIBS        ON) # only find static libs
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    include_directories(${Boost_INCLUDE_DIRS})
    #target_link_libraries( codex.tests ${Boost_LIBRARIES})
    add_definitions( -DENABLE_LIB_BOOST )
endif()

add_subdirectory( googletest )
add_subdirectory( codex )
add_subdirectory( codex/tests )
<<<<<<< HEAD
add_subdirectory( asio/echo )
=======
>>>>>>> b02660fe0ec9398afd6e93fce659808111b1e61a
