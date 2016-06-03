/**
 */
#ifndef __codex_pipe_h__
#define __codex_pipe_h__

#include <codex/codex.hpp>

#if !defined( __codex_win32__ )

namespace codex {

  class pipe {
  public:
    pipe( void );
    ~pipe( void );

    int read_pipe(void);
    int write_pipe(void);
  private:
    int _pipe[2]; 
  };
}

#endif

#endif
