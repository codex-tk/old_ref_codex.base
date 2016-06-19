#ifndef __codex_io_ip_socket_ops_h__
#define __codex_io_ip_socket_ops_h__

#include <codex/codex.hpp>
#include <codex/io/buffer.hpp>
#include <codex/io/ip/address.hpp>

#include <fcntl.h>

namespace codex{ namespace io{ namespace ip{

  template < 
#if defined( __codex_win32__ )
    class SocketType = SOCKET
#else
    class SocketType = int
#endif
  >
  class socket_ops{
  public:
    typedef SocketType socket_type;

    static SocketType invalid() {
#if defined ( __codex_win32__ )
      return INVALID_SOCKET;
#else
      return -1;
#endif
    }

    static SocketType socket(int af, int type, int protocol) {
#if defined ( __codex_win32__ )
      return ::WSASocketW(af, type, protocol , NULL , NULL , WSA_FLAG_OVERLAPPED );
#else
      return ::socket( af , type , protocol);
#endif
    }
    
    static bool blocking( SocketType fd ) {
#if defined ( __codex_win32__ )
      unsigned long opt = 0;
      return ioctlsocket( fd , FIONBIO, &opt ) != 1;
#else
      int opt = fcntl( fd , F_GETFL , 0 );
      return fcntl( fd , F_SETFL , opt & ~O_NONBLOCK ) != -1;
#endif
    }

    static bool nonblocking( SocketType fd ) {
#if defined ( __codex_win32__ )
      unsigned long opt = 1;
      return ioctlsocket( fd , FIONBIO, &opt ) != 1;
#else
      int opt = fcntl( fd , F_GETFL , 0 );
      return fcntl( fd , F_SETFL , opt | O_NONBLOCK ) != -1;
#endif
    }

    template < class Address >
    static bool connect( SocketType fd , const Address& addr ){
      int r = -1;
      do {
        r = ::connect( fd , addr.sockaddr() , addr.length() );
      } while ((r==-1) && (errno == EINTR ));
      if ( ( r == 0 ) || ( errno == EINPROGRESS ) ) {
        return true;
      }
      return false;
    }

    static int writev( SocketType fd , const codex::io::buffer* buf , int cnt ) {
      int r = -1;
      do {
        r = ::writev( fd , buf , cnt );
      }while((r==-1)&&(errno==EINTR));
      return r;
    }

    static int readv( SocketType fd , const codex::io::buffer* buf , int cnt ) {
      int r = -1;
      do {
        r = ::readv( fd , buf , cnt );
      }while((r==-1)&&(errno==EINTR));
      return r;
    }

    static void close(SocketType& fd) {
#if defined ( __codex_win32__ )
      ::closesocket(fd);
#else
      ::close(fd);
#endif
      fd = invalid();
    }

    static bool reuseaddr( SocketType fd ) {
      int opt = 1;
      return setsockopt( fd , SOL_SOCKET , SO_REUSEADDR 
          , reinterpret_cast<char*>(&opt) , sizeof(opt)) != -1; 
    }

    template < class Address >
    static bool bind(SocketType fd, const Address& addr) {
      int r = -1;
      do {
        r = ::bind(fd, addr.sockaddr(), addr.length());
      } while ((r == -1) && (errno == EINTR));
      return r != -1;
    }

    static bool listen( SocketType fd , int backlog ){
      int r = -1;
      do {
        r = ::listen(fd, backlog);
      } while ((r == -1) && (errno == EINTR));
      return r != -1;
    }

    template < class Address >
    static SocketType accept( SocketType fd , Address& addr ) {
      SocketType accepted = -1;
      do {
        accepted = ::accept(fd, addr.sockaddr() , addr.length_ptr());
      } while ((accepted == -1) && (errno == EINTR));
      return accepted; 
    }

    template < class Address >
    static int sendto( SocketType fd , codex::io::buffer& buf , Address& addr ) {
      int r = -1;
      do {
        r = ::sendto( fd , buf.ptr() , buf.length() , 0 , addr.sockaddr() , addr.length() );
      }while((r==-1)&&(errno==EINTR));
      return r;
    }

    template < class Address >
    static int recvfrom( SocketType fd , codex::io::buffer& buf , Address& addr ) {
      int r = -1;
      do {
        r = ::recvfrom( fd , buf.ptr() , buf.length() , 0 , addr.sockaddr() , addr.length_ptr() );
      }while((r==-1)&&(errno==EINTR));
      return r;
    }
  };
}}}

#endif


