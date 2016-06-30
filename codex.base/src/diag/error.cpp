#include <codex/diag/error.hpp>

namespace codex {

  const char* error_category_impl::name( void ) const noexcept {
    return "codex";
  }

  std::string error_category_impl::message( int ev ) const {
    static std::vector< std::string > map {
      std::string( "success" ) ,
      std::string( "disconnect" ) ,
      std::string( "closed_by_user" ) ,
    };
    if ( ev < static_cast<int>(map.size()) ) 
      return map[ev];
    return std::string("error");
  }

  std::error_condition error_category_impl::default_error_condition( int ev ) const noexcept {
    /*
    if ( ev == static_cast<int>(codex::errc::disconnect))
      return std::errc::connection_reset;
    */
    return std::error_condition( ev , *this );
  }

  bool error_category_impl::equivalent( const std::error_code& code , int cond ) const noexcept{
    /*
    if ( cond == static_cast<int>(codex::errc::disconnect))
      return code == std::errc::connection_reset;
    */
    return false;
  }
  
  const std::error_category& category( void ) {
    static error_category_impl impl;
    return impl;
  }

  std::error_code make_error_code( codex::errc ec ) {
    return std::error_code( static_cast<int>(ec) , codex::category());
  }

  std::error_condition make_error_condition( codex::errc ec ){
    return std::error_condition( static_cast<int>(ec) , codex::category());
  }

  std::error_code last_error( void ) {
#if defined( __codex_win32__)
    return std::error_code( GetLastError() , std::system_category());
#else
    return std::error_code( errno , std::system_category());
#endif
  }

}

namespace std {

}
