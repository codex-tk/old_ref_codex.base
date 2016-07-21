/**
 */
#ifndef __codex_proactor_proactor_h__
#define __codex_proactor_proactor_h__ 

#include <codex/codex.hpp>
#include <codex/proactor/iocp.hpp>

namespace codex { namespace proactor {

#if defined( __codex_linux__ )

#elif defined( __codex_apple__ )

#elif defined( __codex_win32__ )
  typedef codex::proactor::iocp implementation_type;
#endif

  class engine{
  public:
    engine( void );
    ~engine( void );

    implementation_type& implementation( void );

    int wait( const int waitms );
    void wakeup( void );
  private:
    proactor::implementation_type _implementation;
  };

}}

#endif
