/*
 */
#ifndef __codex_reactor_async_layer_h__
#define __codex_reactor_async_layer_h__

namespace codex {
  class loop;
}

namespace codex { namespace reactor {

  class async_layer {
  public:
    struct descriptor;
    typedef std::shared_ptr< descriptor > descriptor_type;
  public:
    async_layer( codex::loop& l );
    ~async_layer( void );

    descriptor_type wrap( int native_fd );
       
    codex::loop& loop( void );

  public:
    template < class Handler >
    void connect( descriptor_type& desc , 
        const codex::io::ip::tcp::address& address , 
        Handler&& handler )
    {
    }

  private:
    codex::loop& _loop;
  };

}}

#endif
