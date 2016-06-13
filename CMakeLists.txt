CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

project( codex.base.workspace )

#option( MyOption "MyOption" OFF )
#cmake -DMyOption=ON project -D...

option( CODEX_SHARED "Shared Build Option" OFF )

if(MSVC)
	add_compile_options("/wd4819")
	add_compile_options("/wd4127")
endif()

add_subdirectory( googletest )

add_subdirectory( codex.base )

add_subdirectory( codex.base.tests )

add_subdirectory( exam/echoserver )
