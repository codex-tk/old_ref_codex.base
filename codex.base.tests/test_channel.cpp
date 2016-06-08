#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/loop.hpp>
#include <codex/io/ip/tcp/reactor_channel.hpp>


TEST( channel , tcp ){
  codex::loop l;
  codex::io::ip::tcp::reactor_channel channel( l );

  int fd;

//  channel.bind( fd );
}
