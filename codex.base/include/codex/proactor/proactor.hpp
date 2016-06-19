/**
 */
#ifndef __codex_proactor_proactor_h__
#define __codex_proactor_proactor_h__ 

#include <codex/codex.hpp>
#include <codex/proactor/iocp.hpp>
#include <codex/io/ip/tcp/proactor_channel.hpp>
#include <codex/io/ip/tcp/proactor_channel_builder.hpp>

namespace codex { namespace proactor {

#if defined( __codex_linux__ )

#elif defined( __codex_apple__ )

#elif defined( __codex_win32__ )
  typedef codex::proactor::iocp impl;
#endif


  class engine{
  public:
    engine( void );
    ~engine( void );

    impl& impl( void );

    int wait( const int waitms );
    void wakeup( void );
  private:
    proactor::impl _impl;
  };

}}

#endif
