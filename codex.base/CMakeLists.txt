project( codex.base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

if(MSVC)
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
else()
	add_definitions( -Wall -std=c++11 -o2 -g -ggdb )
endif()


set( SRCS
	include/codex/predefs.hpp
	include/codex/codex.hpp
	include/codex/allocator.hpp )


set( SRCS
  	src/codex.cpp
		src/allocator.cpp )

add_library( codex.base STATIC ${SRCS} )

if ( CODEX_SHARED )
	add_library( codex.base.shared SHARED ${SRCS} )
endif()
