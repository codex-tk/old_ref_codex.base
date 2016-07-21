#include <codex/io/operation.hpp>
/*
namespace codex { namespace io {

  operation::operation( io_execute_fp iofn 
      , codex::operation< void () >::execute_fp handler_fn )
    : codex::operation< void () >(handler_fn)
    , _io_execute( iofn )
  {
  }

  operation::~operation( void ){
  }

  bool operation::operator()( async_layer* layer ){
    if ( _io_execute ) 
      return _io_execute( this , layer );
    return false;
  }

}}
*/
