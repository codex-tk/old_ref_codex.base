#include <codex/log/writer.hpp>

namespace codex { namespace log {

  writer::writer( void )
      : _formatter( default_formatter() )
  {
      
  }

  writer::~writer( void ){

  }

  formatter_ptr writer::formatter( void ) {
      return _formatter;
  }

  void writer::formatter( const formatter_ptr& f ){
      _formatter = f;
  }

}}
