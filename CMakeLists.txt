CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )

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
	add_definitions( -Wall -std=c++1z  -o2 -g -ggdb -Wc++11-extensions -Wunneeded-internal-declaration )
endif()

add_subdirectory( googletest )
add_subdirectory( codex )
add_subdirectory( codex/tests )