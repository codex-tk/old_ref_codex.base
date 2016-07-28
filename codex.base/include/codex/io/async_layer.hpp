/*!
 */
#ifndef __codex_io_async_layer_h__
#define __codex_io_async_layer_h__

#include <codex/io/ip/socket_ops.hpp>

namespace codex{
  class loop;
}

namespace codex { namespace io {

  class operation;
  class async_layer{
  public:
    struct _descriptor;
    typedef std::shared_ptr< _descriptor > descriptor_type;
  public:
    static descriptor_type open( codex::loop& l );
    static void native_descriptor( const descriptor_type& fd 
        , codex::io::ip::socket_ops<>::socket_type sfd );
  public:
    static void connect( const descriptor_type& fd 
        , struct sockaddr* addr_ptr 
        , socklen_t addr_len
        , codex::io::operation* op );


  public:

  };


}}

#endif
