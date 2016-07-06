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
    struct descriptor;
    typedef std::shared_ptr< descriptor > descriptor_type;
  public:
    async_layer( loop& l , engine& e );
    ~async_layer( void );

    descriptor_type wrap( int native_fd );
       
  private:
    loop& _loop;
    engine& _engine;
  };

}}

#endif
