/*!
 */
#ifndef __codex_io_ip_tcp_socket_h__
#define __codex_io_ip_tcp_socket_h__

#include <codex/io/async_layer.hpp>
#include <codex/io/ip/tcp/operation/connect_operation.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {


  class socket{
  public:
    socket( codex::loop& l );
    ~socket( void );

    template < class Address , class Handler >
    void connect( const Address& addr , Handler&& h ); 
    
    template < std::size_t N , class Handler >
    void read( const std::array< codex::io::buffer , N >& buf , Handler&& h );
  private:
    async_layer::descriptor_type _fd;
  };

  template < class Address , class Handler >
  void socket::connect( const Address& address , Handler&& h ) {
    tcp::connect_operation< Address , Handler >* op
      = codex::io::alloc_operation< tcp::connect_operation< Address , Handler > >(
          _fd , address , std::forward< Handler >(h));
    async_layer::native_descriptor( _fd , 
        codex::io::ip::socket_ops<>::socket( address ));
    async_layer::connect( _fd 
        , op->address().sockaddr()
        , op->address().length()
        , op );
  }
 
  template < std::size_t N , class Handler >
  void socket::read( const std::array< codex::io::buffer , N >& buf , Handler&& h ){

  }

}}}}

#endif
