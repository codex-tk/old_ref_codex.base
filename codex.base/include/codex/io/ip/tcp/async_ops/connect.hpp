/*
 */
#ifndef __codex_io_ip_tcp_connect_op_h__
#define __codex_io_ip_tcp_connect_op_h__

namespace codex { namespace io { namespace ip { namespace tcp {

  class connect_op : public codex::io::operation {
  public:
    connect_op( void );
    ~connect_op( void );

    static bool handle_connect( io::operation* op , async_layer* layer );
    void handle_connect( async_layer* l );
  private:
    async_layer::descriptor_type _descriptor;
    codex::io::ip::tcp::address _address;
  };

}}}}

#endif
