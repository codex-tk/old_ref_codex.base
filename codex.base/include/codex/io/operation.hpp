/**
 */
#ifndef __codex_io_operation_h__
#define __codex_io_operation_h__

#include <codex/operation.hpp>
#include <codex/io/async_layer.hpp>

namespace codex { namespace io {

class operation 
  : public codex::operation< void () > {
public:
  typedef codex::operation< void () > base_operation_type;
  typedef codex::operation< void () >::handler_type handler_type;
  typedef bool (*io_handler_type)( io::operation* );
public:
  operation( handler_type handler 
      , io_handler_type io_handler
      , const codex::io::async_layer::descriptor_type& fd );
  ~operation( void );

  using codex::operation< void () >::operator();

  bool handle_io( void );

  void error( const std::error_code& ec );
  std::error_code error( void );
public:
  
private:
  std::error_code _error;
  io_handler_type _io_handler;
  codex::io::async_layer::descriptor_type _fd;
};

template < class OperationType , class... Args >
OperationType* alloc_operation( Args&&... args ) {
  return new OperationType( std::forward<Args>(args)... );
}

template < class OperationType >
void free_operation( OperationType* op ) {
  delete op;
}


}}

#endif
