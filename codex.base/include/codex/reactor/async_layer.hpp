/*
 */
#ifndef __codex_reactor_async_layer_h__
#define __codex_reactor_async_layer_h__

namespace codex {
  class loop;
}

namespace codex { namespace reactor {

  class engine;
  class async_layer {
  public:
    async_layer( loop& l , engine& e );
    ~async_layer( void );

  private:
    loop& _loop;
    engine& _engine;
  };

}}

#endif
