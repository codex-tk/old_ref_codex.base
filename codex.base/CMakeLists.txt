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

if ( MSVC )

else()
  if ( APPLE )
    set( PLATFORM_DEPEND_HEADERS 
      include/codex/reactor/kqueue.hpp
    )
    set( PLATFORM_DEPEND_SRCS 
      src/reactor/kqueue.cpp
    )
  else()
    set( PLATFORM_DEPEND_HEADERS 
      include/codex/reactor/epoll.hpp
    )
    set( PLATFORM_DEPEND_SRCS 
      src/reactor/epoll.cpp
    )
  endif()
endif()

set( HEADERS 
	include/codex/predefs.hpp
	include/codex/codex.hpp
	include/codex/allocator.hpp 
	include/codex/loop.hpp 
  include/codex/operation.hpp
  include/codex/slist.hpp
  include/codex/reactor/reactor.hpp
  ${PLATFORM_DEPEND_HEADERS}
  include/codex/pipe.hpp
  include/codex/reactor/pipe_wakeup.hpp
  include/codex/reactor/eventfd_wakeup.hpp
  include/codex/reactor/poll_handler.hpp
  include/codex/io/buffer.hpp
  include/codex/io/ip/tcp/reactor_channel.hpp
  include/codex/buffer/ctrl_blk.hpp
  include/codex/buffer/shared_blk.hpp
  include/codex/buffer/packetizer.hpp
  )


set( SRCS
  	src/codex.cpp
		src/allocator.cpp 
    src/loop.cpp
    src/reactor/reactor.cpp
    ${PLATFORM_DEPEND_SRCS}
    src/pipe.cpp
    src/reactor/pipe_wakeup.cpp
    src/reactor/eventfd_wakeup.cpp
    src/reactor/poll_handler.cpp
    src/io/buffer.cpp
    src/io/ip/tcp/reactor_channel.cpp
    src/buffer/ctrl_blk.cpp
    src/buffer/shared_blk.cpp
    src/buffer/packetizer.cpp
    )

  add_library( codex.base STATIC ${HEADERS} ${SRCS} )

if ( CODEX_SHARED )
  add_library( codex.base.shared SHARED ${HEADERS} ${SRCS} )
endif()
