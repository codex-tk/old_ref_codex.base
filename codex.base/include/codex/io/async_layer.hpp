/*!
 */
#ifndef __codex_io_async_layer_h__
#define __codex_io_async_layer_h__

#include <codex/io/ip/socket_ops.hpp>

namespace codex{
  class loop;
}

namespace codex { namespace io {

  class async_layer{
  public:
    struct _descriptor;
    typedef std::shared_ptr< _descriptor > descriptor_type;
  public:
    static descriptor_type open( codex::loop& l );
//    descriptor_type open( codex::loop& l , io::ip::socket_ops<>::socket_type fd );

  public:



  public:

  };


}}

#endif
