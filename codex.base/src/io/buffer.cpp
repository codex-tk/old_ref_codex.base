#include <codex/io/buffer.hpp>

namespace codex { namespace io {

  buffer::buffer( void ) {
    ptr( nullptr );
    length(0);
  }

  buffer::buffer( const iovec& iov ){
#if defined( __codex_win32__ )
    buf = iov.buf;
    len = iov.len;
#else
    iov_base = iov.iov_base;
    iov_len = iov.iov_len;
#endif
  }

  buffer::buffer( const buffer& rhs ){
    ptr( rhs.ptr() );
    length( rhs.length() );
  }

  buffer::buffer( void* p , int l ){
    ptr(static_cast<char*>(p));
    length(l);
  }

  char* buffer::ptr( void ) const {
#if defined( __codex_win32__ )
    return buf;
#else
    return static_cast<char*>(iov_base);
#endif
  }

  int buffer::length( void ) const {
#if defined( __codex_win32__ )
    return len;
#else
    return iov_len;
#endif
  }

  void buffer::ptr( char* p ) {
#if defined( __codex_win32__ )
    buf = p;
#else
    iov_base = p;
#endif
  }

  void buffer::length( int l ) {
#if defined( __codex_win32__ )
    len = l;
#else
    iov_len = l;
#endif
  }

  void buffer::skip( int l ) {
    ptr( ptr() + l );
    length( length() - l );
  }

}}

