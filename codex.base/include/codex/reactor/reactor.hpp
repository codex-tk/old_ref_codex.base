/**
 */
#ifndef __codex_reactor_reactor_h__
#define __codex_reactor_reactor_h__ 

#include <codex/codex.hpp>

#if defined( __codex_linux__ )
#include <codex/reactor/epoll.hpp>
#include <codex/reactor/eventfd_wakeup.hpp>
#else

#endif

namespace codex { namespace reactor {
namespace{
#if defined( __codex_linux__ )
  typedef codex::reactor::epoll poller;
  typedef codex::reactor::eventfd_wakeup wakeup; 
#elif defined( __codex_apple__ )

#elif defined( __codex_win32__ )

#endif
}

  class engine{
  public:
    engine( void );
    ~engine( void );

    poller& reactor( void );

    int wait( const int waitms );
    void wakeup( void );
  private:
    reactor::poller _poller;
    reactor::wakeup _wakeup;
  };

}}

#endif
