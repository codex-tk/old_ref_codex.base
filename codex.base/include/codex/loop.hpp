/**
 */
#ifndef __codex_loop_h__
#define __codex_loop_h__

#include <vector>

#include <codex/codex.hpp>
#include <codex/operation.hpp>

namespace codex{

  /**
   */
  class loop {
  public:
    typedef codex::operation< void () > operation_type;
    typedef typename operation_type::pointer_type operation_ptr;
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
    void post( const operation_ptr& ptr ); 
  private:
    /**
     */
    void drain_ops(void);

  public:
    /**
     */
    template < class Handler >
    static operation_ptr wrap( Handler&& h ) {
      return operation_type::make_shared( 
          std::forward< Handler >(h));
    }

  private:
    codex::threading::mutex _lock;
    std::shared_ptr< std::vector< operation_ptr > > _ops;
    std::shared_ptr< std::vector< operation_ptr > > _ops_reserve;
  };
}

#endif
