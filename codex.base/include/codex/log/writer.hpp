#ifndef __codex_log_writer_h__
#define __codex_log_writer_h__

#include <codex/log/formatter.hpp>

namespace codex {  namespace log {

  struct record;
  class writer{
  public:
    writer( void );
    virtual ~writer( void );
    virtual void write( const record& r ) = 0;
    formatter_ptr formatter( void );
    void formatter( const formatter_ptr& f );
  private:
    formatter_ptr _formatter;
  };

  typedef std::shared_ptr< writer > writer_ptr;

}}

#endif

