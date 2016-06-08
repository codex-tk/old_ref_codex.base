#include <sys/eventfd.h> 

#include <codex/reactor/eventfd_wakeup.hpp>

#if defined( __codex_linux__ ) 

namespace codex { namespace reactor {

  eventfd_wakeup::eventfd_wakeup( void )
    : _eventfd( eventfd( 0 , O_NONBLOCK ) )
    , _handler( &eventfd_wakeup::handler_callback )
  {
    _handler.events( codex::reactor::pollin );
  }
  
  eventfd_wakeup::~eventfd_wakeup( void ){
  }

  void eventfd_wakeup::set( void ) {
    eventfd_t val = 1;
    eventfd_write( _eventfd , val );
  }

  void eventfd_wakeup::reset( void ) {
    eventfd_t val = 0;
    eventfd_read( _eventfd , &val );
  }

  int eventfd_wakeup::handle( void ) {
    return _eventfd;
  }

  codex::reactor::poll_handler* eventfd_wakeup::handler( void ) {
    return &_handler;
  }

  void eventfd_wakeup::handler_callback( codex::reactor::poll_handler* ev 
      , const int events ) 
  {
    eventfd_wakeup* ptr = codex::container_of( ev , &eventfd_wakeup::_handler );
    if ( events & codex::reactor::pollin ) 
      ptr->reset();
  }
  
}}

#endif
