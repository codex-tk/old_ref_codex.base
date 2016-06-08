#include <codex/reactor/pipe_wakeup.hpp>

#if !defined( __codex_win32__ ) 

namespace codex { namespace reactor {

  pipe_wakeup::pipe_wakeup( void )
    : _handler( &pipe_wakeup::handler_callback )
  {
    _handler.events( codex::reactor::pollin );
  }
  
  pipe_wakeup::~pipe_wakeup( void ){
  }

  void pipe_wakeup::set( void ) {
    char ch = '0';
    ::write( _pipe.write_pipe() , &ch , 1 );
  }

  void pipe_wakeup::reset( void ) {
    char buf[64];
    ::read( _pipe.read_pipe() , buf , 64 );
  }

  int pipe_wakeup::handle( void ) {
    return _pipe.read_pipe();
  }

  codex::reactor::poll_handler* pipe_wakeup::handler( void ) {
    return &_handler;
  }

  void pipe_wakeup::handler_callback( codex::reactor::poll_handler* ev 
      , const int polls ) 
  {
    pipe_wakeup* ptr = codex::container_of( ev , &pipe_wakeup::_handler );
    if ( polls & codex::reactor::pollin ) 
      ptr->reset();
  }
  
}}

#endif
