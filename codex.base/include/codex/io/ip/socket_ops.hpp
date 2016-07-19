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

    static SocketType invalid_socket;
    static SocketType socket_error;

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
      fd = invalid_socket;
    }

    static bool reuseaddr( SocketType fd ) {
      int opt = 1;
      return ::setsockopt( fd , SOL_SOCKET , SO_REUSEADDR 
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

    class option {
    public:
      template < typename T , int level , int opt > class value_option {
      public:
        value_option( void ) {
        }
        explicit value_option( const T& t ) : _option(t) {
        }
        ~value_option( void ) {
        }
        bool set_option( SocketType fd ) {
          return ::setsockopt( fd , 
              level , 
              opt , 
              reinterpret_cast< char*>(&_option) , 
              sizeof( _option ) ) != socket_error;
        }
        bool get_option( SocketType fd ) {
          socklen_t size = sizeof( _option );
          return ::getsockopt( fd,
              level , 
              opt , 
              reinterpret_cast< char*>(&_option) ,
              &size ) != socket_error;
        }
        T& value( void ) {
          return _option;
        }
        void value( const T& value ) {
          _option = value;
        }
      private:
        T _option;
      };
    public:
      typedef value_option< int   , SOL_SOCKET  , SO_REUSEADDR    > reuse_address;
      typedef value_option< int   , SOL_SOCKET  , SO_BROADCAST    > broad_cast;
      typedef value_option< int   , SOL_SOCKET  , SO_RCVBUF       > recv_buffer;
      typedef value_option< int   , SOL_SOCKET  , SO_RCVTIMEO     > recv_time;
      typedef value_option< int   , SOL_SOCKET  , SO_SNDBUF       > send_buffer;
      typedef value_option< int   , SOL_SOCKET  , SO_SNDTIMEO     > send_time;
      typedef value_option< int   , SOL_SOCKET  , SO_ERROR        > error; 
      typedef value_option< struct linger , SOL_SOCKET  , SO_LINGER > linger;

      static linger linger_remove_time_wait;
      typedef value_option< uint8_t , IPPROTO_IP  , IP_MULTICAST_LOOP   > multi_cast_loop;
      typedef value_option< uint8_t , IPPROTO_IP  , IP_MULTICAST_TTL    > multi_cast_ttl;
      typedef value_option< in_addr , IPPROTO_IP  , IP_MULTICAST_IF   > multi_cast_if;
      typedef value_option< ip_mreq , IPPROTO_IP  , IP_ADD_MEMBERSHIP   > multi_cast_join;
      typedef value_option< ip_mreq , IPPROTO_IP  , IP_DROP_MEMBERSHIP  > multi_cast_drop;
      typedef value_option< uint8_t , IPPROTO_IPV6  , IPV6_MULTICAST_LOOP > multi_cast_loop_ipv6;
      typedef value_option< uint8_t , IPPROTO_IPV6  , IPV6_MULTICAST_HOPS > multi_cast_ttl_ipv6;
      typedef value_option< in6_addr  , IPPROTO_IPV6  , IPV6_MULTICAST_IF   > multi_cast_if_ipv6;
#if !defined( __codex_apple__ )
      typedef value_option< ipv6_mreq , IPPROTO_IPV6  , IPV6_ADD_MEMBERSHIP > multi_cast_join_ipv6;
      typedef value_option< ipv6_mreq , IPPROTO_IPV6  , IPV6_DROP_MEMBERSHIP  > multi_cast_drop_ipv6;
#endif

      typedef value_option< int , IPPROTO_TCP , TCP_NODELAY   > tcp_no_delay;
#if defined( __codex_win32_h__ )
      typedef value_option< BOOL   , SOL_SOCKET  , SO_CONDITIONAL_ACCEPT   > conditional_accept;
      typedef value_option< int    , SOL_SOCKET  , SO_CONNECT_TIME     > connect_time;
      typedef value_option< SOCKET , SOL_SOCKET  , SO_UPDATE_ACCEPT_CONTEXT  > update_accept_context;
#endif
    };
  
    template < class Option >
    static bool setsockopt( SocketType fd , Option& opt ) {
      return opt.set_option(fd);
    }

    template < class Option >
    static bool getsockopt( SocketType fd , Option& opt ) {
      return opt.get_option(fd);
    }

  };

  template < class SocketType >
  typename socket_ops< SocketType >::socket_type
#if defined( __codex_win32__ )
    socket_ops<SocketType>::invalid_socket = INVALID_SOCKET;
#else
    socket_ops<SocketType>::invalid_socket = -1 ;
#endif

  template < class SocketType >
  typename socket_ops< SocketType >::socket_type
#if defined( __codex_win32__ )
    socket_ops<SocketType>::socket_error = SOCKET_ERROR;
#else
    socket_ops<SocketType>::socket_error = -1 ;
#endif


}}}

#endif


