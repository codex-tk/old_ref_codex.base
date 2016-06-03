/**
 */
#ifndef __codex_reactor_reactor_h__
#define __codex_reactor_reactor_h__ 

#include <codex/codex.hpp>

namespace codex { namespace reactor {

  enum events {
    pollin = 0x01,
    pollout = 0x04,
  };

  class event_handler {
  public:
    typedef void (*callback)( event_handler* handler , const int events );
    event_handler( callback cb );
    ~event_handler( void );
    void operator()( int events );
  private:
    callback _callback;
  };

}}

#endif
