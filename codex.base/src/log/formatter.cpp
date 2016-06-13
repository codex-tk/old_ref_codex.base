#include <codex/log/formatter.hpp>
#include <codex/log/record.hpp>
#include <stdio.h>

namespace codex { namespace log {

  formatter::formatter( void ) {

  }

  formatter::~formatter( void ) {

  }

  void formatter::format( const record& r , codex::buffer::shared_blk& buf ){
    codex::systemtime st(r.ts);
    if ( buf.size() < 8192 ) {
      codex::buffer::shared_blk nblk( 8192 );
      memcpy( nblk.write_ptr() , buf.read_ptr() , buf.length() );
      nblk.write_ptr( buf.length());
      nblk.swap(buf);
    }
#if defined( __codex_win32__ )
    int len = _snprintf_s(reinterpret_cast<char*>( buf.write_ptr()) , buf.space() , _TRUNCATE
#else
    int len = snprintf(reinterpret_cast<char*>( buf.write_ptr()) , buf.space() 
#endif
          , "[%04d%02d%02d %02d%02d%02d][%c][%s][%s][%s][%s:%d][%d]\n"
          , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
          , acronym(r.type)
          , r.tag
          , r.message
          , r.function
          , r.file
          , r.line
          , r.tid
      );
    buf.write_ptr( len );
  }

 formatter_ptr default_formatter( void ){
    static formatter_ptr ptr( new formatter());
    return ptr;
 }

}}
