/**
 */
#ifndef __codex_operation_h__
#define __codex_operation_h__

#include <memory>

namespace codex {

  template < class T >
  class operation;

  template < class R , class ...Args>
  class operation< R ( Args... ) > {
  public:
    operation( void ){
    }
    virtual ~operation( void ) {
    }
    virtual R operator()( Args&&... arg ) = 0;
  public:
    typedef std::shared_ptr< operation > pointer_type;

    template < class Handler >
    static pointer_type make_shared( Handler&& handler ){
      class _op : public operation {
        public:
          _op( Handler&& h ) 
            : _handler( std::forward<Handler>( h ) ){
            }

          virtual ~_op( void ) {
          }

          virtual R operator()( Args&&... arg ) {
            return _handler( std::forward< Args >(arg)... );  
          }

        private:
          Handler _handler;
      };
      return std::make_shared< _op > ( std::forward< Handler >(handler));
    }
  };


}

#endif
