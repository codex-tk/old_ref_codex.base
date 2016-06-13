namespace codex { namespace io { namespace ip {

  template < int Type , int Protocol >
  address< Type , Protocol >::address( void )
  : _length( sizeof(_address)){
    memset( &_address , 0x00 , sizeof( _address ));
  }

  template < int Type , int Protocol >
  address< Type , Protocol >::address( struct sockaddr* addr , socklen_t len )
  : _length( len ) 
  {
    memcpy( &_address , addr , len );
  }

  template < int Type, int Protocol >
  address< Type, Protocol >::address(int af, int port)
  {
    switch (af) {
      case AF_INET: 
        {
          struct sockaddr_in addr;
          memset(&addr, 0x00, sizeof(addr));
          addr.sin_addr.s_addr = INADDR_ANY;
          addr.sin_port = htons((unsigned short)port);
          addr.sin_family = AF_INET;
          memcpy(&_address, &addr, sizeof(addr));
          _length = sizeof(addr);
        }
        break;
      case AF_INET6: 
        {
          // todo
          /*
             struct sockaddr_in6 addr;
             memset(&addr, 0x00, sizeof(addr));
             addr.sin_addr.s_addr = INADDR_ANY;
             addr.sin_port = htons(port);
             addr.sin_family = AF_INET6;
             memcpy(&_address, &addr, sizeof(addr));
             _length = sizeof(addr);*/
        }
        break;
    }
  }

  template < int Type , int Protocol >
  address< Type , Protocol >::address( const address& addr ) 
  : _length( addr.length())
  {
    memcpy( &_address , &addr._address , _length );
  }

  template < int Type , int Protocol >
  address< Type , Protocol >::~address( void ){}

  template < int Type , int Protocol >
  sockaddr* address< Type , Protocol >::sockaddr( void ) const {
    return reinterpret_cast< struct sockaddr* >( 
        const_cast< struct sockaddr_storage* >( &_address));
  }

  template < int Type , int Protocol >
  socklen_t address< Type , Protocol >::length( void ) const {
    return _length;
  }

  template < int Type , int Protocol >
  socklen_t* address< Type , Protocol >::length_ptr( void ) {
    return &_length;
  }

  template < int Type , int Protocol >
  int address< Type , Protocol >::family( void ) const {
    return ((struct sockaddr_in*)sockaddr())->sin_family;
  }

  template < int Type , int Protocol >
  int address< Type , Protocol >::port( void ) const {
    return ntohs(((struct sockaddr_in*)sockaddr())->sin_port);
  }

  template < int Type , int Protocol >
  int address< Type , Protocol >::type( void ) const {
    return Type;
  }

  template < int Type , int Protocol >
  int address< Type , Protocol >::protocol( void ) const {
    return Protocol;
  }


  template < int Type , int Protocol >
  std::vector< address<Type,Protocol> > 
  address< Type , Protocol >::resolve( const char* name , int port , int family ){
    struct addrinfo hints;
    struct addrinfo* result = nullptr;
    struct addrinfo* rp = nullptr;

    memset( &hints , 0x00 , sizeof( hints ));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;

    char port_s[16];

    sprintf( port_s , "%d" , port );

    std::vector< address > resolved;
    if ( getaddrinfo( name , port_s , &hints , &result ) != 0 )
      return resolved;

    for ( rp = result ; rp != nullptr ; rp = rp->ai_next ){
      address a( rp->ai_addr , (int)(rp->ai_addrlen));
      resolved.push_back( a );
    }
    freeaddrinfo( result );
    return resolved;  
  }

  template < int Type , int Protocol >
  address<Type,Protocol> address<Type,Protocol>::from_ipaddress( const char* ipaddr
      , const int port )
  {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipaddr);
    addr.sin_port = htons( port );
    return address<Type,Protocol>( reinterpret_cast< struct sockaddr* >(&addr) , sizeof(addr));
  }

}}}
