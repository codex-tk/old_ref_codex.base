#include <codex/codex.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/tcp/acceptor.hpp>
#include <codex/io/ip/tcp/reactor_channel.hpp>
#include <codex/log/log.hpp>

class echo_handler : public codex::io::ip::tcp::event_handler{
public:
  echo_handler( codex::io::ip::tcp::reactor_channel* chan ) 
    : _channel(chan){
  }
  virtual ~echo_handler( void ){
    LOG_D( "echo" , "Delete" );
  }

  virtual void on_read( codex::buffer::shared_blk blk ){
    LOG_D( "echo" , "on read : %d" , (int)blk.length());
    _channel->write(blk);
  }

  virtual void on_write( const int write_bytes , bool flushed ) {
    LOG_D( "echo" , "on write: %d" , write_bytes);
  }

  virtual void on_error( void ) {
    LOG_D( "echo" , "on error");
    _channel->close();
  }
private:
  codex::io::ip::tcp::reactor_channel* _channel;
};

int main( int argv , char* argc[] ) {
  codex::loop l;
  codex::io::ip::tcp::acceptor acceptor(l);
  codex::log::logger::instance().enable( codex::log::debug );
  codex::log::logger::instance().add_writer( codex::log::console_writer::instance());
  acceptor.open(7543);
  acceptor.set_on_accept( 
      [&]( int fd , const codex::io::ip::tcp::address& addr ) {
        LOG_D( "echo" , "Accept" );
        codex::io::ip::tcp::reactor_channel* chan =
          new codex::io::ip::tcp::reactor_channel( l );
        chan->bind( fd , std::make_shared< echo_handler >(chan));
      });
  while ( true ) {
    l.dispatch(100);
  }
  return 0;
}
