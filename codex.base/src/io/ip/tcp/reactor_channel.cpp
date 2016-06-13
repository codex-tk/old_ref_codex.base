#include <codex/io/ip/tcp/reactor_channel.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/socket_ops.hpp>
#include <codex/log/log.hpp>
#include <codex/diag/error.hpp>
#include <codex/io/ip/tcp/reactor_channel_builder.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  namespace {
    int k_handle_error_bit = 0x10000000;
  }

  event_handler::event_handler(void){
  }

  event_handler::~event_handler(void){
  }

  void event_handler::channel_ptr( tcp::channel_ptr ptr ) {
    _channel_ptr = ptr;
  }

  tcp::channel_ptr& event_handler::channel_ptr( void ){
    return _channel_ptr;
  }

  void event_handler::on_error0( const std::error_code& ec ){
    on_error( ec );
    _channel_ptr.reset();
  }

  reactor_channel::reactor_channel( void )
    : _fd(-1)
    , _poll_handler( &reactor_channel::handle_event0 )
    , _ref_count( k_handle_error_bit | 1 )
    , _loop( nullptr )
    , _builder( nullptr )
  {
  }

  reactor_channel::~reactor_channel( void ) {

  }

  codex::loop& reactor_channel::loop( void ) {
    return *_loop;
  }

  int reactor_channel::bind( int fd ) {
    _fd = fd;
    _poll_handler.events(codex::reactor::pollin);
    codex::io::ip::socket_ops<int>::nonblocking( _fd );
    return _loop->engine().reactor().bind( _fd , &_poll_handler );
  }

  void reactor_channel::close( void ) {
    add_ref();
    _loop->post_handler( [this]{
        handle_error( codex::make_error_code( codex::errc::closed_by_user ));
        release();
    });
  }

  void reactor_channel::write( codex::buffer::shared_blk blk ){
    if ( blk.length() <= 0 )
      return;
    add_ref();
    _loop->post_handler( [this,blk]{
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
      _builder->on_end_reference(this);
    }
    return cnt;
  }

  void reactor_channel::reset( void ) {
    _ref_count = k_handle_error_bit | 1;
    _fd = -1;
    _write_packets.clear();
    _handler.reset();
    _packetizer.reset();
  }

  void reactor_channel::set_builder( reactor_channel_builder* builder ) {
    _builder = builder;
  }
  void reactor_channel::set_loop( codex::loop* loop ) {
    _loop = loop;
  }
  void reactor_channel::set_packetizer( 
      const std::shared_ptr< codex::buffer::packetizer >& packetizer ){
    _packetizer = packetizer;
  }
  
  void reactor_channel::set_handler( const std::shared_ptr< event_handler >& handler ) {
    _handler = handler;
  }

  std::error_code reactor_channel::handle_pollin( void ) {
    if ( _fd == -1 ) 
      return std::make_error_code( std::errc::bad_file_descriptor );
    if ( (_ref_count.load() & k_handle_error_bit) == 0 ) {
      return std::make_error_code( std::errc::owner_dead );
    }

    codex::io::buffer buf[32];
    int iovcnt = _packetizer->setup( buf , 32 );
    int readbytes = codex::io::ip::socket_ops<int>::readv( _fd , buf , iovcnt );
    if ( readbytes < 0 )
      return std::error_code( errno , std::system_category() ); 
    if ( readbytes == 0 )
      return codex::make_error_code( codex::errc::disconnect );
      
    _packetizer->assemble( readbytes );
    while ( _packetizer->done() ) {
      auto blk = _packetizer->packet();
      if ( _handler ) {
        _handler->on_read( blk );
      }
    }
    return std::error_code();
  }

  std::error_code reactor_channel::handle_pollout( void ) {
    if ( _fd == -1 ) 
      return std::make_error_code( std::errc::bad_file_descriptor );
    if ( (_ref_count.load() & k_handle_error_bit) == 0 ) {
      return std::make_error_code( std::errc::owner_dead );
    }
    int iovcnt = static_cast<int>( _write_packets.size());
    codex::io::buffer buf[iovcnt];
    for ( int i = 0 ; i < iovcnt ; ++i ){
      buf[i].ptr( static_cast<char*>(_write_packets[i].read_ptr()));
      buf[i].length( static_cast<int>(_write_packets[i].length()));
    }
    int writebytes = codex::io::ip::socket_ops< int >::writev( _fd , buf , iovcnt );
    if ( writebytes < 0 ) {
      return std::error_code( errno , std::system_category() ); 
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
      _loop->engine().reactor().bind( _fd , &_poll_handler );
    
    if ( _handler )
      _handler->on_write( on_write , _write_packets.empty() );
    return std::error_code();
  }

  void reactor_channel::write0( codex::buffer::shared_blk blk ){
    _write_packets.push_back( blk );
    reactor_channel::handle_event0( &_poll_handler , codex::reactor::pollout );
  }

  void reactor_channel::handle_error( const std::error_code& ec ) {
    if ( _ref_count.load() & k_handle_error_bit) {
      do {
        int expected = _ref_count.load();
        int desired = expected & ~k_handle_error_bit; 
        if ( _ref_count.compare_exchange_strong(expected,desired))
          break;
      } while(true);
      _loop->engine().reactor().unbind( _fd );
      if ( _handler ) {
        _handler->on_error0(ec);
      }
    }
    if ( ec == codex::errc::closed_by_user ) {
      if ( _fd != -1 ) {
        ip::socket_ops<int>::close( _fd );
      }
      _packetizer->clear();
      _write_packets.clear();
      release();
    }
  }

  void reactor_channel::handle_event0( codex::reactor::poll_handler* p 
      , const int events )
  {
    reactor_channel* chan = codex::container_of( p , &reactor_channel::_poll_handler );
    if ( chan ) {
      std::error_code ec;
      if ( events & codex::reactor::pollin ) 
        ec = chan->handle_pollin();
      if ( ( events & codex::reactor::pollout ) && !ec ) 
        ec = chan->handle_pollout();
      if ( ec ) {
        chan->handle_error(ec);
      }
    }
  }

}}}}
