/**
 */
#ifndef __codex_reactor_poll_handler_h__
#define __codex_reactor_poll_handler_h__

#include <codex/codex.hpp>

namespace codex { namespace reactor {

  enum poll_events{
    pollin = 0x01,
    pollout = 0x04,
  };

  class poll_handler {
  public:
    typedef void (*callback)( poll_handler* handler , const int polls );
    poll_handler( callback cb );
    ~poll_handler( void );
    void operator()( int polls );

    int events( void );
    void events( const int evt );
  private:
    callback _callback;
    int _events;
  };

}}

#endif
