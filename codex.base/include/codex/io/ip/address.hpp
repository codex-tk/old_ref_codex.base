#ifndef __codex_io_ip_address_h__
#define __codex_io_ip_address_h__

#include <codex/codex.hpp>

#if !defined( __codex_win32__ )
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

namespace codex { namespace io { namespace ip {

  template < int Type , int Protocol >
  class address {
  public:
    address( void );
    address( struct sockaddr* addr , socklen_t len );
    address( const address& addr );
    // any 
    address(int af, int port);
    address& operator=( const address& addr );

    ~address( void );

    struct sockaddr* sockaddr( void ) const;
    socklen_t length( void ) const;
    socklen_t* length_ptr( void );

    int family( void ) const;
    int port( void ) const;

    int type( void ) const;
    int protocol( void ) const;
  public:
    static std::vector< address > resolve( const char* name , int port , int family = AF_UNSPEC );
    static address from_ipaddress( const char* ipaddr , const int port );
  private:
    struct sockaddr_storage _address;
    socklen_t _length;
  };

  namespace tcp {
    using address = ip::address< SOCK_STREAM , IPPROTO_TCP >;
  }
  namespace udp {
    using address = ip::address< SOCK_DGRAM , IPPROTO_UDP >;
  }

}}}

#include <codex/io/ip/address.ipp>

#endif


