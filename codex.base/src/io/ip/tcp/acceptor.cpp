#include <codex/io/ip/tcp/acceptor.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/socket_ops.hpp>
#include <codex/log/log.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  acceptor::acceptor( codex::loop& l )
    : _loop(l) 
    , _poll_handler( &acceptor::handle_event0 )
    , _fd( -1 ) 
  {
    
  }

  acceptor::~acceptor( void ){

  }

  int acceptor::open( const int port ) {
    close();
    codex::io::ip::tcp::address addr( AF_INET , port );
    _fd = socket( addr.family() , addr.type() , addr.protocol() );
    if ( _fd != -1 ) {
      if ( ip::socket_ops<int>::nonblocking( _fd ) ) {
        if ( ip::socket_ops<int>::reuseaddr( _fd ) ) {
          if ( ip::socket_ops<int>::bind( _fd , addr )) {
            if( ip::socket_ops<int>::listen( _fd , SOMAXCONN ) ){
              _poll_handler.events(codex::reactor::pollin);
              _loop.engine().reactor().bind( _fd , &_poll_handler );
              return 0;
            }
          }
        }
      }
    }
    close();
    return -1;
  }

  void acceptor::close( void ) {
    if ( _fd != -1 ) {
      _loop.engine().reactor().unbind(_fd);
      ::close(_fd);
    }
  }

  void acceptor::handle_pollin( void ){
    codex::io::ip::tcp::address addr;
    int fd = ip::socket_ops<int>::accept( _fd , addr );
    if ( fd != -1 ) {
      _on_accept( fd , addr );
    }
  }

  void acceptor::handle_event0( codex::reactor::poll_handler* p , const int events ) {
    LOG_D( "acceptor" , "handle" );
    acceptor* obj = codex::container_of( p , &acceptor::_poll_handler );
    if ( obj ) {
      if ( events & codex::reactor::pollin ) 
        obj->handle_pollin();
    }
  }

}}}}
