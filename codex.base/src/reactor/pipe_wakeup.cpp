#include <codex/reactor/pipe_wakeup.hpp>

#if !defined( __codex_win32__ ) 

namespace codex { namespace reactor {

  pipe_wakeup::pipe_wakeup( void )
    : _handler( &pipe_wakeup::handler_callback )
  {
  }
  
  pipe_wakeup::~pipe_wakeup( void ){
  }

  void pipe_wakeup::set( void ) {
    char ch = '0';
    ::write( _pipe.write_pipe() , &ch , 1 );
  }

  void pipe_wakeup::reset( void ) {
    char ch;
    ::read( _pipe.read_pipe() , &ch , 1 );
  }

  int pipe_wakeup::handle( void ) {
    return _pipe.read_pipe();
  }

  codex::reactor::event_handler* pipe_wakeup::handler( void ) {
    return &_handler;
  }

  void pipe_wakeup::handler_callback( codex::reactor::event_handler* ev 
      , const int events ) 
  {
    pipe_wakeup* ptr = codex::container_of( ev , &pipe_wakeup::_handler );
    if ( events & codex::reactor::pollin ) 
      ptr->reset();
  }
  
}}

#endif
