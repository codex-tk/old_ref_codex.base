project( exam_echoserver )

include_directories( ${codex.base_SOURCE_DIR}/include )

link_directories( ${codex.base.workspace_BINARY_DIR}/codex.base )

if( MSVC )
	add_definitions (/D _CRT_SECURE_NO_WARNINGS)
else()
	add_definitions( -Wall -std=c++11 -o2 -g -ggdb )
	if( APPLE )

	else()

	endif()
endif()

add_executable( exam_echoserver 
  main.cpp
)

target_link_libraries( exam_echoserver 
 codex.base
)
