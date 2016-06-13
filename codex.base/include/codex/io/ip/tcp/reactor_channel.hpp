#ifndef __codex_io_ip_tcp_reactor_channel_h__
#define __codex_io_ip_tcp_reactor_channel_h__

#include <codex/codex.hpp>
#include <codex/reactor/poll_handler.hpp>
#include <codex/buffer/packetizer.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace io { namespace ip { namespace tcp {

  class event_handler {
  public:
    event_handler( void );
    virtual ~event_handler( void );
    virtual void on_read( codex::buffer::shared_blk blk ) = 0 ;
    virtual void on_write( const int write_bytes , bool flushed ) = 0;
    virtual void on_error( const std::error_code& ec ) = 0;
  };

  class reactor_channel {
  public:
    explicit reactor_channel( codex::loop& l  );
    reactor_channel( codex::loop& l 
        , std::shared_ptr< codex::buffer::packetizer > pkt ); 
    ~reactor_channel( void );

    codex::loop& loop( void );

    int bind( int fd , std::shared_ptr< event_handler > handler );
    void close( void );

    void write( codex::buffer::shared_blk blk );

    int add_ref( void );
    int release( void );
  private:
    static void handle_event0( codex::reactor::poll_handler* poll , const int events );
    std::error_code handle_pollin( void );
    std::error_code handle_pollout( void );
    void handle_error( const std::error_code& ec );
    void handle_end_reference( void );
    void write0( codex::buffer::shared_blk blk );
  private:
    codex::loop& _loop;
    codex::reactor::poll_handler _poll_handler;
    int _fd;
    std::shared_ptr< codex::buffer::packetizer > _packetizer;
    std::deque< codex::buffer::shared_blk > _write_packets;
    codex::threading::atomic<int> _ref_count;
    std::shared_ptr< event_handler > _handler;
  };

}}}}

#endif
