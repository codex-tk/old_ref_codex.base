/**
 */
#ifndef __codex_loop_h__
#define __codex_loop_h__

#include <vector>
#include <thread>

#include <codex/codex.hpp>
#include <codex/operation.hpp>
#include <codex/slist.hpp>
#if defined( __codex_win32__ )
#include <codex/proactor/proactor.hpp>
#else
#include <codex/reactor/reactor.hpp>
#endif
#include <codex/io/async_layer.hpp>

namespace codex{
  
  /**
   */
  class loop {
  public:
    typedef codex::operation< void () > operation_type;
#if defined( __codex_win32__ )
    typedef codex::proactor::engine engine_type;
#else
    typedef codex::reactor::engine engine_type;
#endif
  public:
    /**
     */
    loop( void );
    /**
     */
    ~loop( void );

    /**
     */
    int dispatch( const int waitms );

    /**
     */
    void post( operation_type* ptr ); 

    /**
     */
    template < class Handler >
    void post_handler( Handler&& h ) {
      post( operation_type::wrap( std::forward< Handler >(h)));
    }

    codex::loop::engine_type& engine( void );

    bool in_loop( void );
  private:
    /**
     */
    int drain_ops(void);
    int drain_in_loop_ops(void);
  private:
    codex::threading::mutex _lock;
    codex::loop::engine_type _engine;
    std::thread::id _loop_id;
    codex::slist< operation_type > _ops;
    codex::slist< operation_type > _in_loop_ops;
  };
}

#endif
