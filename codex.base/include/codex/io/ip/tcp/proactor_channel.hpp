#ifndef __codex_io_ip_tcp_proactor_channel_h__
#define __codex_io_ip_tcp_proactor_channel_h__

#include <codex/codex.hpp>
#include <codex/proactor/proactor.hpp>
#include <codex/buffer/packetizer.hpp>
#include <codex/io/ip/socket_ops.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace io { namespace ip { namespace tcp {

  class proactor_channel;
  class proactor_channel_builder;

  typedef std::shared_ptr< proactor_channel > channel_ptr;

  class event_handler {
  public:
    event_handler( void );
    virtual ~event_handler( void );
    virtual void on_read( codex::buffer::shared_blk blk ) = 0 ;
    virtual void on_write( const int write_bytes , bool flushed ) = 0;
    virtual void on_error( const std::error_code& ec ) = 0;
    
    void channel_ptr( tcp::channel_ptr ptr );
    tcp::channel_ptr& channel_ptr( void );
    void on_error0( const std::error_code& ec );
  private:
    tcp::channel_ptr _channel_ptr;
  };

  class proactor_channel {
  public:
    proactor_channel( void );
    ~proactor_channel( void );

    codex::loop& loop( void );

    int bind( int fd );
    void close( void );
    bool closed( void );

    void write( codex::buffer::shared_blk blk );

    int add_ref( void );
    int release( void );
  public:
    void reset( void );
    void set_builder( proactor_channel_builder* builder );
    void set_loop( codex::loop* loop );
    void set_packetizer( const std::shared_ptr< codex::buffer::packetizer >& packetizer );
    void set_handler( const std::shared_ptr< event_handler >& handler );
  private:
    static void handle_read0( void* ctx 
      , const std::error_code& ec 
      , const int io_bytes );
    static void handle_write0(void* ctx
      , const std::error_code& ec
      , const int io_bytes);
    void handle_read(const std::error_code& ec
      , const int io_bytes);
    void handle_write(const std::error_code& ec
      , const int io_bytes);
    void handle_error( const std::error_code& ec );
    void handle_end_reference( void );
    void write0( codex::buffer::shared_blk blk );

    void do_read(void);
    void do_write(void);
  private:
    io::ip::socket_ops<>::socket_type _fd;
    codex::proactor::iocp::handler _read_handler;
    codex::proactor::iocp::handler _write_handler;
    codex::threading::atomic<int> _ref_count;
    codex::loop* _loop;
    std::shared_ptr< event_handler > _handler;
    std::shared_ptr< codex::buffer::packetizer > _packetizer;
    std::deque< codex::buffer::shared_blk > _write_packets;
    proactor_channel_builder* _builder;
  };


}}}}

#endif
