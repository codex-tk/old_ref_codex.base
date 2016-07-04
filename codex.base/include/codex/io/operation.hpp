/**
 */
#ifndef __codex_io_operation_h__
#define __codex_io_operation_h__

#include <codex/operation.hpp>

namespace codex { namespace io {

class async_layer;
class operation : public codex::operation< void () > {
public:
  typedef bool (*io_execute_fp)( io::operation* op , async_layer* layer );
public:
  operation( io_execute_fp iofn 
      , codex::operation< void () >::execute_fp handler_fn );

  ~operation( void );

  bool operator()( async_layer* layer );
private:
  io_execute_fp _io_execute;
};

}}

#endif