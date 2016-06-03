#include <codex/pipe.hpp>

#if !defined( __codex_win32__ )

namespace codex {

  pipe::pipe( void ){
    ::pipe( _pipe );
  }
  
  pipe::~pipe( void ) {
    ::close( _pipe[0] );
    ::close( _pipe[1] );
  }

  int pipe::read_pipe(void) {
    return _pipe[0];
  }   

  int pipe::write_pipe(void) {
    return _pipe[1];
  }
}

#endif
