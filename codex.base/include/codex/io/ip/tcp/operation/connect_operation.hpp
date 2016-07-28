/*!
 */
#ifndef __codex_io_ip_tcp_connect_operation_h__
#define __codex_io_ip_tcp_connect_operation_h__

#include <codex/io/operation.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  template < class Address , class Handler >
  class connect_operation : public codex::io::operation {
  public:
    connect_operation( 
        const codex::io::async_layer::descriptor_type& fd 
        , const Address& address 
        , Handler&& h )
      : codex::io::operation(
            &connect_operation::handler_impl 
            , nullptr 
            , fd ) 
        , _address( address )
        , _handler( std::forward<Handler>(h) ) 
    {
    }

    Address& address( void ) {
      return _address;
    }

    static void handler_impl( base_operation_type * op ) {
      connect_operation* ptr = static_cast< connect_operation* > (op);
      Handler handler( std::move( ptr->_handler ));
      Address addr( std::move( ptr->_address ));
      std::error_code ec( ptr->error());
      codex::io::free_operation( ptr );
      handler( ec , addr );  
    }
  private:
    Address _address;
    Handler _handler;
  };

}}}}

#endif
