#include <codex/io/operation.hpp>
namespace codex { namespace io {

  operation::operation( codex::operation< void () >::handler_type handler 
      , io_handler_type io_handler
      , const codex::io::async_layer::descriptor_type& fd )
    : codex::operation< void () >( handler )
    , _io_handler( io_handler )
    , _fd( fd )
  {

  }
  operation::~operation( void ){

  }

  bool operation::handle_io( void ){
    if ( _io_handler )
      return _io_handler(this);
    return true;
  }

  void operation::error( const std::error_code& ec ){
    _error = ec;
  }

  std::error_code operation::error( void ){
    return _error;
  }

}}


