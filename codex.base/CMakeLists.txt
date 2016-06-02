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


set( HEADERS 
	include/codex/predefs.hpp
	include/codex/codex.hpp
	include/codex/allocator.hpp 
	include/codex/loop.hpp 
  include/codex/operation.hpp
  )


set( SRCS
  	src/codex.cpp
		src/allocator.cpp 
    src/loop.cpp
    )

  add_library( codex.base STATIC ${HEADERS} ${SRCS} )

if ( CODEX_SHARED )
  add_library( codex.base.shared SHARED ${HEADERS} ${SRCS} )
endif()
