#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/tcp/reactor_channel.hpp>
#include <memory>


TEST( channel , tcp ){
  codex::loop l;
//  codex::io::ip::tcp::reactor_channel channel( l );

//  int fd;

//  channel.bind( fd );
}

/*
struct value : public std::enable_shared_from_this<value> {
  int val; 
};
TEST( ptr , t0 ) {
  value p;
  { 
    std::shared_ptr< value > pint( &p, []( value* ) {
      });
  }
  std::shared_ptr< value > vv = p.shared_from_this();
  ASSERT_TRUE( vv );
}
*/
