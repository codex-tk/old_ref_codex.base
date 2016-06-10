#ifndef __codex_io_buffer_h__
#define __codex_io_buffer_h__

#include <codex/codex.hpp>

#if defined( __codex_win32__ )
typedef WSABUF iovec;
#else
#include <sys/uio.h>
#endif

namespace codex { namespace io {

  class buffer : public iovec{
  public:
    buffer( void );
    buffer( const iovec& iov );
    buffer( const buffer& rhs );
    buffer( void* p , int l );

    char* ptr( void ) const ;
    int length( void ) const ;

    void ptr( char* p ) ;
    void length( int l ) ;
    void skip( int l ) ;
  };

}}

#endif
