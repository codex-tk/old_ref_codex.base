project( codex.base.tests )

include_directories( ${codex.base_SOURCE_DIR}/include )
include_directories( ${gtest_SOURCE_DIR}/include )

link_directories( ${codex.base.workspace_BINARY_DIR}/codex.base )

if( MSVC )
	add_definitions (/D _CRT_SECURE_NO_WARNINGS)
else()
	add_definitions( -Wall -std=c++11 -o2 -g -ggdb )
	if( APPLE )

	else()

	endif()
endif()

add_executable( codex.base.tests
	test_codex.cpp
	test_allocator.cpp
  test_loop.cpp
  test_reactor.cpp
  test_channel.cpp
  test_buffer.cpp
  test_error.cpp
)

target_link_libraries( codex.base.tests
 codex.base
 gtest
 gtest_main
)
