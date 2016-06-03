/**
 */
#ifndef __codex_loop_h__
#define __codex_loop_h__

#include <vector>

#include <codex/codex.hpp>
#include <codex/operation.hpp>
#include <codex/slist.hpp>

namespace codex{

  /**
   */
  class loop {
  public:
    typedef codex::operation< void () > operation_type;
  public:
    /**
     */
    loop( void );
    /**
     */
    ~loop( void );

    /**
     */
    int dispatch( void );

    /**
     */
    void post( operation_type* ptr ); 
  private:
    /**
     */
    int drain_ops(void);
  private:
    codex::threading::mutex _lock;
    codex::slist< operation_type > _ops;
  };
}

#endif
