/**
 * license
 */

#ifndef __codex_reactor_pipe_wakeup_h__
#define __codex_reactor_pipe_wakeup_h__

#if !defined( __codex_win32__ ) 

#include <codex/pipe.hpp>
#include <codex/reactor/reactor.hpp>

namespace codex { namespace reactor {

  /**
   */
  class pipe_wakeup{
  public:
    pipe_wakeup( void );
    ~pipe_wakeup( void );

    void set( void );

    void reset( void );

    int handle( void );
    codex::reactor::event_handler* handler( void );
  private:
    static void handler_callback( codex::reactor::event_handler* ev 
        , const int events );
  private:
    codex::pipe _pipe; 
    codex::reactor::event_handler _handler;
  };
}}

#endif

#endif
