#include <gtest/gtest.h>
#include <codex/loop.hpp>
#include <codex/io/async_layer.hpp>
#include <codex/io/ip/tcp/socket.hpp>
#include <codex/diag/error.hpp>

TEST( async_socket , connect ) {
  codex::loop l;
  codex::io::ip::tcp::socket s(l);
  auto addrs = codex::io::ip::tcp::address::resolve( "google.co.kr" , 80 ); 
  //s.connect( codex::io::ip::tcp::address::from_ipaddress( "0.0.0.0" , 0 )
  std::error_code ec_result( codex::make_error_code( codex::errc::bad_file_descriptor ));
  bool run = true;
  s.connect( addrs[0]
      , [&] ( const std::error_code& ec , const codex::io::ip::tcp::address& addr ) {
        ec_result  = ec;
        run = false;
      });
  while( run )  
    l.dispatch(1000);
  ASSERT_TRUE( !ec_result );

}


