project( codex.base )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

if ( MSVC )
    set( PLATFORM_DEPEND_HEADERS
        include/codex/proactor/iocp.hpp
				include/codex/proactor/proactor.hpp
				include/codex/io/ip/tcp/proactor_channel.hpp
    )
    set( PLATFORM_DEPEND_SRCS
        src/proactor/iocp.cpp
				src/proactor/proactor.cpp
				src/io/ip/tcp/proactor_channel.cpp
    )
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
	  set( PLATFORM_DEPEND_HEADERS
        ${PLATFORM_DEPEND_HEADERS}
        include/codex/pipe.hpp
        include/codex/reactor/reactor.hpp
        include/codex/reactor/pipe_wakeup.hpp
        include/codex/reactor/eventfd_wakeup.hpp
        include/codex/reactor/poll_handler.hpp
        include/codex/io/ip/tcp/reactor_channel.hpp
        include/codex/reactor/async_layer.hpp
    )
    set( PLATFORM_DEPEND_SRCS
        ${PLATFORM_DEPEND_SRCS}
        src/reactor/reactor.cpp
        src/reactor/pipe_wakeup.cpp
        src/reactor/eventfd_wakeup.cpp
        src/reactor/poll_handler.cpp
        src/io/ip/tcp/reactor_channel.cpp
        src/reactor/async_layer.cpp
     )

endif()

set( HEADERS
    include/codex/predefs.hpp
    include/codex/codex.hpp
    include/codex/allocator.hpp
    include/codex/loop.hpp
    include/codex/operation.hpp
    include/codex/slist.hpp
    include/codex/timestamp.hpp
    include/codex/timespan.hpp
    include/codex/io/buffer.hpp
    include/codex/io/ip/address.hpp
    include/codex/io/ip/address.ipp
		include/codex/io/ip/socket_ops.hpp
    include/codex/io/ip/tcp/acceptor.hpp
    include/codex/buffer/ctrl_blk.hpp
    include/codex/buffer/shared_blk.hpp
    include/codex/buffer/packetizer.hpp
    include/codex/log/record.hpp
    include/codex/log/writer.hpp
    include/codex/log/logger.hpp
    include/codex/log/formatter.hpp
    include/codex/log/console_writer.hpp
    include/codex/diag/error.hpp
    ${PLATFORM_DEPEND_HEADERS}
		include/codex/io/ip/tcp/channel.hpp
		include/codex/io/ip/tcp/channel_builder.hpp
  )


set( SRCS
    src/codex.cpp
    src/allocator.cpp
    src/loop.cpp
    src/timestamp.cpp
    src/timespan.cpp
    src/io/buffer.cpp
		src/io/ip/socket_ops.cpp
    src/io/ip/tcp/acceptor.cpp
    src/buffer/ctrl_blk.cpp
    src/buffer/shared_blk.cpp
    src/buffer/packetizer.cpp
    src/log/record.cpp
    src/log/writer.cpp
    src/log/logger.cpp
    src/log/formatter.cpp
    src/log/console_writer.cpp
    src/diag/error.cpp
    ${PLATFORM_DEPEND_SRCS}
		src/io/ip/tcp/channel.cpp
		src/io/ip/tcp/channel_builder.cpp
    )

  add_library( codex.base STATIC ${HEADERS} ${SRCS} )

if ( CODEX_SHARED )
  add_library( codex.base.shared SHARED ${HEADERS} ${SRCS} )
endif()
