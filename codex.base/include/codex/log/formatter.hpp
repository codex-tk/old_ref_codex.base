#ifndef __codex_log_formatter_h__
#define __codex_log_formatter_h__

#include <codex/buffer/shared_blk.hpp>
#include <memory>

namespace codex {  namespace log {

  struct record;
  class formatter {
  public:
    formatter( void );
    virtual ~formatter( void );
    virtual void format( const record& r , codex::buffer::shared_blk& buf );
  };

  typedef std::shared_ptr< formatter > formatter_ptr;

  formatter_ptr default_formatter( void );

}}

#endif

