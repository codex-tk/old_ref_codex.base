#include <codex/io/ip/tcp/reactor_channel.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/socket_ops.hpp>
#include <codex/log/log.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  namespace {
    int k_error_bit = 0x10000000;
  }

  event_handler::event_handler(void){
  }

  event_handler::~event_handler(void){
  }

  reactor_channel::reactor_channel( codex::loop& l ) 
    : _loop( l )
    , _poll_handler( &reactor_channel::handle_event0 )
    , _fd(-1)
    , _packetizer( std::make_shared< codex::buffer::random_packetizer >( 4096 ) )
    , _ref_count( k_error_bit | 1 )
  {
  }

  reactor_channel::~reactor_channel( void ) {

  }

  codex::loop& reactor_channel::loop( void ) {
    return _loop;
  }

  int reactor_channel::bind( int fd , std::shared_ptr< event_handler > handler ) {
    if ( _fd != -1 ) 
      _loop.engine().reactor().unbind( _fd );
    _fd = fd;
    _poll_handler.set(codex::reactor::pollin);
    _handler = handler;
    codex::io::ip::socket_ops<int>::nonblocking( _fd );
    return _loop.engine().reactor().bind( _fd , &_poll_handler );
  }

  void reactor_channel::close( void ) {
    add_ref();
    _loop.post_handler( [this]{
        handle_error(0);
        release();
        });
  }

  void reactor_channel::write( codex::buffer::shared_blk blk ){
    if ( blk.length() <= 0 )
      return;
    add_ref();
    _loop.post_handler( [this,blk]{
        write0(blk);
        release();
        });
  }

  int reactor_channel::add_ref( void ) {
    return _ref_count.fetch_add(1);
  }

  int reactor_channel::release( void ) {
    int cnt = _ref_count.fetch_sub(1);
    if ( cnt == 1 ){
      delete this;
    }
    return cnt;
  }

  int reactor_channel::handle_pollin( void ) {
    codex::io::buffer buf[32];
    int iovcnt = _packetizer->setup( buf , 32 );
    int readbytes = codex::io::ip::socket_ops<int>::readv( _fd , buf , iovcnt );
    if ( readbytes <= 0 )
      return -1;
      
    _packetizer->assemble( readbytes );
    while ( _packetizer->done() ) {
      auto blk = _packetizer->packet();
      if ( _handler ) {
        _handler->on_read( blk );
      }
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
    int writebytes = codex::io::ip::socket_ops< int >::writev( _fd , buf , iovcnt );
    if ( writebytes < 0 ) {
      return -1;
    }
    int on_write = writebytes;
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
    
    if ( _handler )
      _handler->on_write( on_write , _write_packets.empty() );
    return 0;
  }

  void reactor_channel::write0( codex::buffer::shared_blk blk ){
    _write_packets.push_back( blk );
    reactor_channel::handle_event0( &_poll_handler , codex::reactor::pollout );
  }

  void reactor_channel::handle_error( const int error ) {
    if ( _ref_count.load() & k_error_bit ) {
      do {
        int expected = _ref_count.load();
        int desired = expected & ~k_error_bit; 
        if ( _ref_count.compare_exchange_strong(expected,desired))
          break;
      } while(true);
      _loop.engine().reactor().unbind( _fd );
      if ( _handler ) {
        _handler->on_error();
      }
    }
    if ( error == 0 )
      release();
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
        chan->handle_error(ret);
      }
    }
  }

}}}}
