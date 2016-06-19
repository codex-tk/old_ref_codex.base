#include <codex/codex.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/tcp/acceptor.hpp>
#include <codex/io/ip/tcp/channel.hpp>
#include <codex/log/log.hpp>

#if defined( __codex_win32__ )

#pragma comment( lib , "ws2_32")
#pragma comment( lib , "mswsock")

#endif


class echo_handler : public codex::io::ip::tcp::event_handler{
public:
  virtual ~echo_handler( void ){
    LOG_D( "echo" , "Delete" );
  }
  virtual void on_read( codex::buffer::shared_blk blk ){
    LOG_D( "echo" , "on read : %d" , (int)blk.length());
    channel_ptr()->write( blk );
  }

  virtual void on_write( const int write_bytes , bool flushed ) {
    LOG_D( "echo" , "on write: %d" , write_bytes);
  }

  virtual void on_error( const std::error_code& ec ) {
    channel_ptr()->close();
    LOG_D( "echo" , "on error: %s" , ec.message().c_str() );
  }
};

class builder : public codex::io::ip::tcp::channel_builder {
public:
  builder( codex::loop& l ) : _loop(l){
  }
  virtual codex::loop& loop( void ) {
    return _loop;
  }
  virtual std::shared_ptr< codex::io::ip::tcp::event_handler > handler(){
    return std::make_shared< echo_handler >(); 
  }
private:
  codex::loop& _loop;
};

int main( int argv , char* argc[] ) {
#if defined( __codex_win32__ )
  WSAData ws;
  WSAStartup(MAKEWORD(2, 2), &ws);
#endif
  codex::loop l;
  builder b(l);
  codex::io::ip::tcp::acceptor acceptor(l);
  codex::log::logger::instance().enable( codex::log::debug );
  if ( acceptor.open(7543) >= 0 ) {
    LOG_D( "echo" , "open" );
    acceptor.set_on_accept( 
        [&]( int fd , const codex::io::ip::tcp::address& addr ) {
          LOG_D( "echo" , "Accept" );
          auto chan = b.build();
          chan->bind( fd );
        });
    while ( true ) {
      int cnt  = l.dispatch(1000);
    }
  }
  LOG_D( "echo" , "Error %s" , std::error_code( GetLastError()
    , std::system_category()).message().c_str() );
  return 0;
}
