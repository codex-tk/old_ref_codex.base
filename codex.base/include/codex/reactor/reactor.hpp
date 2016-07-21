/**
 */
#ifndef __codex_reactor_reactor_h__
#define __codex_reactor_reactor_h__ 

#include <codex/codex.hpp>

#if defined( __codex_linux__ )
#include <codex/reactor/epoll.hpp>
#include <codex/reactor/eventfd_wakeup.hpp>
#elif defined( __codex_apple__ )
#include <codex/reactor/kqueue.hpp>
#include <codex/reactor/pipe_wakeup.hpp>
#endif

namespace codex { namespace reactor {
#if defined( __codex_linux__ )
  typedef codex::reactor::epoll implementation_type;
  typedef codex::reactor::eventfd_wakeup wakeup; 
#elif defined( __codex_apple__ )
  typedef codex::reactor::kqueue implementation_type;
  typedef codex::reactor::pipe_wakeup wakeup;
#elif defined( __codex_win32__ )

#endif

  class engine{
  public:
    engine( void );
    ~engine( void );

    reactor::implementation_type& implementation(void);

    int wait( const int waitms );
    void wakeup( void );
  private:
    reactor::implementation_type _implementation;
    reactor::wakeup _wakeup;
  };

}}

#endif
