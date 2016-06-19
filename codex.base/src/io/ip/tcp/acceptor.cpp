#include <codex/io/ip/tcp/acceptor.hpp>
#include <codex/loop.hpp>

#include <codex/log/log.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  acceptor::acceptor( codex::loop& l )
    : _loop(l) 
#if defined( __codex_win32__)
    , _handler( &acceptor::handle_accept )
#else
    , _poll_handler(&acceptor::handle_event0)
#endif
    , _fd( ip::socket_ops<>::invalid() ) 
  {
    
  }

  acceptor::~acceptor( void ){

  }

  int acceptor::open( const int port ) {
    close();
    codex::io::ip::tcp::address addr( AF_INET , port );
    _fd = socket_ops<>::socket( addr.family() , addr.type() , addr.protocol() );
    if ( _fd != socket_ops<>::invalid()) {
      if ( ip::socket_ops<>::nonblocking( _fd ) ) {
        if ( ip::socket_ops<>::reuseaddr( _fd ) ) {
          if ( ip::socket_ops<>::bind( _fd , addr )) {
            if( ip::socket_ops<>::listen( _fd , SOMAXCONN ) ){
#if defined( __codex_win32__)
              _loop.engine().impl().bind(_fd, this);
              do_accept();
#else
              _poll_handler.events(codex::reactor::pollin);
              _loop.engine().reactor().bind( _fd , &_poll_handler );
#endif
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
    if ( _fd != ip::socket_ops<>::invalid()) {
#if !defined( __codex_win32__)
      _loop.engine().reactor().unbind(_fd);
#endif
      ip::socket_ops<>::close(_fd);
    }
  }

#if defined( __codex_win32__)
  void acceptor::do_accept(void) {
    codex::io::ip::tcp::address addr(AF_INET, 0);
    _accepted = socket_ops<>::socket( addr.family() , addr.type() , addr.protocol() );
    socket_ops<>::nonblocking(_accepted);
    _handler.reset();
    DWORD dwBytes = 0;
    if (AcceptEx( _fd
      , _accepted
      , _accept_address
      , 0
      , sizeof(sockaddr_storage)
      , sizeof(sockaddr_storage)
      , &dwBytes
      , &_handler) == FALSE)
    {
      std::error_code ec(WSAGetLastError(), std::system_category());
      if (ec.value() != WSA_IO_PENDING) {
        _loop.post_handler([this, ec] {
          handle_accept(ec, 0);
        });
      }
    }
  }

  void acceptor::handle_accept(const std::error_code& ec, const int ) {
    if (ec) {
      ip::socket_ops<>::close(_accepted);
    }
    else {
      struct sockaddr* remote, *local;
      INT remotelen, locallen;
      GetAcceptExSockaddrs(_accept_address,
        0,
        sizeof(sockaddr_storage),
        sizeof(sockaddr_storage),
        &local,
        &locallen,
        &remote,
        &remotelen);
      codex::io::ip::tcp::address local_addr(local, locallen);
      codex::io::ip::tcp::address remote_addr(remote, remotelen);
      _on_accept(_accepted, remote_addr);
    }
    do_accept();
  }
  
  void acceptor::handle_accept(void* ctx, const std::error_code& ec, const int io_bytes) {
    acceptor* obj = static_cast<acceptor*>(ctx);
    if (obj)
      obj->handle_accept(ec, io_bytes);
  }
#else
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
#endif
}}}}
