#include <sys/epoll.h>
#include <unistd.h>

#include <codex/reactor/epoll.hpp>

namespace codex { namespace reactor {

  epoll::epoll( void ) 
    : _epoll_fd( epoll_create( 256 ))
  {
  }

  epoll::~epoll( void ){
    ::close( _epoll_fd );
  }

  bool epoll::bind( int fd 
      , const int events
      , reactor::event_handler* handler )
  {
    epoll_event evt;
    evt.events = events;
    evt.data.ptr = handler;
    if ( epoll_ctl( _epoll_fd , EPOLL_CTL_MOD , fd , &evt ) == 0 )
      return true;
    if ( errno == ENOENT )
      return epoll_ctl( _epoll_fd , EPOLL_CTL_ADD , fd , &evt ) == 0;
    return false;
  }

  void epoll::unbind( int fd ){
    epoll_ctl( _epoll_fd , EPOLL_CTL_DEL , fd , nullptr );
  }
  
  int epoll::wait( const int wait_ms ) {
    struct epoll_event events[256];
    int r = epoll_wait( _epoll_fd , events , 256 , wait_ms );
    if ( r == -1 ) 
      return errno == EINTR ? 0 : -1;
    else {
      for ( int i = 0; i < r ; ++i ) {
        reactor::event_handler* handler = static_cast< reactor::event_handler* >(
            events[i].data.ptr );
        if ( handler ) {
          (*handler)( events[i].events );
        }
      }
    }
    return r;
  }
}}
