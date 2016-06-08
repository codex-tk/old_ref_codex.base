/**
 * license
 */

#ifndef __codex_reactor_eventfd_wakeup_h__
#define __codex_reactor_eventfd_wakeup_h__

#include <codex/codex.hpp>
#include <codex/reactor/poll_handler.hpp>

#if defined( __codex_linux__ ) 

namespace codex { namespace reactor {

  /**
   */
  class eventfd_wakeup{
  public:
    eventfd_wakeup( void );
    ~eventfd_wakeup( void );

    void set( void );

    void reset( void );

    int handle( void );
    codex::reactor::poll_handler* handler( void );
  private:
    static void handler_callback( codex::reactor::poll_handler* ev 
        , const int events );
  private:
    int _eventfd;
    codex::reactor::poll_handler _handler;
  };
}}

#endif

#endif
