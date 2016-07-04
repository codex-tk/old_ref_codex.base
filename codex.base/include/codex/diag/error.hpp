/**
 */
#ifndef __codex_diag_error_h__
#define __codex_diag_error_h__

#include <codex/codex.hpp>

namespace codex { 

  enum class errc {
    success = 0,
    disconnect ,
    closed_by_user ,
    write_buffer_full ,
  };

  class error_category_impl : public std::error_category {
  public:
    virtual const char* name( void ) const noexcept;
    virtual std::string message( int ev ) const;
    virtual std::error_condition default_error_condition( int ev ) const noexcept;
    virtual bool equivalent( const std::error_code& code , int cond ) const noexcept;
  };

  const std::error_category& category( void );

  std::error_code make_error_code( codex::errc ec );

  std::error_condition make_error_condition( codex::errc ec );

  std::error_code last_error( void );

 }

namespace std {
//  template <> struct is_error_code_enum< codex::errc > : public true_type{};
  template <> struct is_error_condition_enum< codex::errc > : public true_type{};
}
#endif
