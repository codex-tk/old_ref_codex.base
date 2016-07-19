#include <sys/event.h>
#include <unistd.h>

#include <codex/reactor/kqueue.hpp>

namespace codex { namespace reactor {

  kqueue::kqueue( void ) 
    : _kqueue_fd( ::kqueue())
  {
  }

  kqueue::~kqueue( void ){
    ::close( _kqueue_fd );
  }

  int kqueue::bind( int fd 
      , reactor::poll_handler* handler )
  {
    if ( handler == nullptr ) 
      return -1;

    struct kevent kev[2];
    int evfilt[2] = { EVFILT_READ , EVFILT_WRITE };
    reactor::poll_events evmask[2] = {
      reactor::poll_events::pollin , 
      reactor::poll_events::pollout 
    };
    memset( kev , 0x00 ,sizeof( kev[0] ) * 2 );
    for ( int i = 0 ; i < 2 ; ++i ) {
      kev[i].ident = fd;
      kev[i].filter = evfilt[i];
      kev[i].flags = EV_ADD;
      kev[i].flags |= (handler->events() & evmask[i] ) ? EV_ENABLE : EV_DISABLE;
      kev[i].udata = handler;
    }
    return ::kevent( _kqueue_fd , kev , 2 , nullptr , 0 , nullptr );
  }

  void kqueue::unbind( int fd ){
    struct kevent kev[2];
    int evfilt[2] = { EVFILT_READ , EVFILT_WRITE };
    memset( kev , 0x00 ,sizeof( kev[0]) * 2 );
    for ( int i = 0 ; i < 2 ; ++i ) {
      kev[i].ident = fd;
      kev[i].filter = evfilt[i];
      kev[i].flags = EV_DELETE;
    }
    ::kevent( _kqueue_fd , kev , 2 , nullptr , 0 , nullptr );
  }
  
  int kqueue::wait( const int wait_ms ) {
    struct kevent ev[256];
    struct timespec ts;
    ts.tv_sec = wait_ms / 1000;
    ts.tv_nsec = ( wait_ms % 1000 ) * 1000000;
    int cnt = ::kevent( _kqueue_fd , nullptr ,  0 , ev , 256 , &ts );
    if ( cnt <= 0 ) 
      return 0;
    for ( int i = 0; i < cnt ; ++i ) {
      reactor::poll_handler* handler = static_cast< reactor::poll_handler* >(
          ev[i].udata);
      if ( handler ) {
        reactor::poll_events e = (ev[i].filter == EVFILT_READ) ?
          reactor::poll_events::pollin : reactor::poll_events::pollout ;
        if ( handler->events() & e ) 
          (*handler)( e );
      }
    }
    return cnt;
  }
}}
