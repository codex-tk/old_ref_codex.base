/*
 */
#ifndef __codex_proactor_async_layer_h__
#define __codex_proactor_async_layer_h__

#include <codex/io/ip/socket_ops.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace proactor{

  class async_layer {
  public:
    typedef SOCKET descriptor_type;
  public:
    async_layer( codex::loop& l );
    ~async_layer( void );

    descriptor_type wrap( SOCKET native_fd );
       
    codex::loop& loop( void );
  private:
    codex::loop& _loop;
  };

}}

#endif
