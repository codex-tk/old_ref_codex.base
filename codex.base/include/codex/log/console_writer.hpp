#ifndef __codex_log_console_writer_h__
#define __codex_log_console_writer_h__

#include <codex/log/writer.hpp>
#include <codex/buffer/shared_blk.hpp>

namespace codex { namespace log {

  class console_writer : public writer {
  public:
    console_writer( void );
    virtual ~console_writer( void );
    virtual void write( const record& r );
  private:  
    codex::buffer::shared_blk _buffer;
  public:
    static std::shared_ptr< writer > instance( void );
  };

}}

#endif


