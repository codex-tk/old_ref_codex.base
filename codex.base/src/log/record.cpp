#include <codex/log/record.hpp>

namespace codex { namespace log {

  char acronym( log::type l ){
    static log::type types[] = {
      trace , debug , info , warn , error ,fatal , all , off
    };
    static char acronyms[] = "TDIWEFAO"; 
    for ( int i = 0 ; i < 8 ; ++i ) {
      if ( l == types[i] ) return acronyms[i];
    }
    return '?';
  }

  record::record( log::type t
      , const char* tag
      , const char* file
      , const char* function
      , const int l )
    : ts( codex::timestamp::now()) ,  type(t) , line(l)
#if defined( __codex_win32__ )
      , tid( GetCurrentThreadId() )
#else
      , tid( pthread_self() )
#endif
  {
#if defined( __codex_win32__ )
    strcpy_s(this->tag , tag );
    strcpy_s(this->file , file  );
    strcpy_s(this->function , function );
#else
    strncpy(this->tag , tag , 32 );
    strncpy(this->file , file  , 256);
    strncpy(this->function , function , 256);
#endif
  }
  
}}
