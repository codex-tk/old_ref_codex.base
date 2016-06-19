/**
 *
 */
#ifndef __codex_proactor_iocp_h__
#define __codex_proactor_iocp_h__

#include <codex/codex.hpp>

namespace codex { namespace proactor {

  class iocp {
  public:
    class handler : public OVERLAPPED {
    public:
      typedef void (*callback)(
          void* ctx 
          , const std::error_code& ec 
          , const int iobytes );
      handler( callback cb );
      void operator()( void* ctx , const std::error_code& ec , const int io_bytes );
      void reset( void );
    private:
      callback _callback;  
    };
  public:
    iocp( void );
    ~iocp( void );

    int bind( SOCKET fd 
        , void* ctx );

    void unbind( SOCKET fd );

    int wait( const int wait_ms );

    void wakeup(void);
  private:
    HANDLE _iocp;
  };

}}

#endif
