#ifndef __codex_log_record_h__
#define __codex_log_record_h__

#include <codex/buffer/shared_blk.hpp>
#include <codex/timestamp.hpp>
#include <codex/bit.hpp>

namespace codex { namespace log {

  enum type {
    trace  = codex::bit(0) , 
    debug  = codex::bit(1) ,
    info   = codex::bit(2) ,
    warn   = codex::bit(3) ,
    error  = codex::bit(4) ,
    fatal  = codex::bit(5) ,
    all    = trace|debug|info|warn|error|fatal,
    off    = 0,
  };

  struct record{
    codex::timestamp ts;
    log::type type;
    uint32_t line;
#if defined( __codex_win32__ )
    int tid;
#else
    pthread_t tid;
#endif
    char tag[32];
    char file[256];
    char function[256];
    char message[2048];

    record( log::type t 
        , const char* tag
        , const char* file
        , const char* function
        , const int line );
  };

  char acronym( log::type l );

}}

#endif
