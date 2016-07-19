#include <gtest/gtest.h>
#include <codex/loop.hpp>
#include <codex/io/ip/tcp/socket.hpp>

TEST( tcp_socket , base ) {
  codex::loop loop;
  codex::io::ip::tcp::socket s(loop);
}
