#include <codex/io/ip/tcp/reactor_channel.hpp>

#include <codex/loop.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  reactor_channel::reactor_channel( codex::loop& l ) 
    : _loop( l )
    , _poll_handler( &reactor_channel::handle_event0 )
    , _fd(-1)
    , _packetizer( std::make_shared< codex::buffer::random_packetizer >( 4096 ) )
  {
  }

  reactor_channel::~reactor_channel( void ) {

  }

  codex::loop& reactor_channel::loop( void ) {
    return _loop;
  }

  int reactor_channel::bind( int fd ) {
    if ( _fd != -1 ) 
      _loop.engine().reactor().unbind( _fd );
    _fd = fd;
    _poll_handler.set(codex::reactor::pollin);
    return _loop.engine().reactor().bind( _fd , &_poll_handler );
  }

  void reactor_channel::write( codex::buffer::shared_blk blk ){
    if ( _loop.in_loop() ) {
      write0( blk );
    } else {
      _loop.post_handler( [this,blk]{
            write0(blk);
          });
    }
  }

  int reactor_channel::handle_pollin( void ) {
    codex::io::buffer buf[32];
    int iovcnt = _packetizer->setup( buf , 32 );
    int readbytes = ::readv( _fd , buf , iovcnt );
    if ( readbytes <= 0 )
      return -1;
      
    if ( readbytes > 0 ) {
      _packetizer->assemble( readbytes );
      while ( _packetizer->done() ) {
        auto blk = _packetizer->packet();
      }
    } else {
      _packetizer->clear();
      // handle error
    }
    return 0;
  }

  int reactor_channel::handle_pollout( void ) {
    int iovcnt = static_cast<int>( _write_packets.size());
    codex::io::buffer buf[iovcnt];
    for ( int i = 0 ; i < iovcnt ; ++i ){
      buf[i].ptr( static_cast<char*>(_write_packets[i].read_ptr()));
      buf[i].length( static_cast<int>(_write_packets[i].length()));
    }
    int writebytes = ::writev( _fd , buf , iovcnt ); 
    if ( writebytes < 0 ) {
      return -1;
    }
    while ( writebytes > 0 ) {
      writebytes -= _write_packets.front().read_ptr( writebytes );
      if ( _write_packets.front().length() == 0 ){
        _write_packets.pop_front();
      }
    }
    int events = _poll_handler.events();
    if ( _write_packets.empty() ) {
      _poll_handler.clear( codex::reactor::pollout );
    } else {
      _poll_handler.set( codex::reactor::pollout );
    }
    if ( events != _poll_handler.events() )
      _loop.engine().reactor().bind( _fd , &_poll_handler );
    return 0;
  }

  void reactor_channel::write0( codex::buffer::shared_blk blk ){
    _write_packets.push_back( blk );
    reactor_channel::handle_event0( &_poll_handler , codex::reactor::pollout );
  }

  void reactor_channel::handle_error( const int error ) {

  }

  void reactor_channel::handle_event0( codex::reactor::poll_handler* p 
      , const int events )
  {
    reactor_channel* chan = codex::container_of( p , &reactor_channel::_poll_handler );
    if ( chan ) {
      int ret = 0;
      if ( events & codex::reactor::pollin ) 
        ret = chan->handle_pollin();
      if ( ( events & codex::reactor::pollout ) && ( ret == 0 ) ) 
        ret = chan->handle_pollout();
      if ( ret != 0 ) {
        chan->handle_error( ret );
      }
    }
  }

}}}}
