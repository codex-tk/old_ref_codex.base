#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/reactor/reactor.hpp>
#include <codex/pipe.hpp>

TEST( reactor , event_handler ) {
  codex::reactor::event_handler handler(nullptr);
}

#if defined ( __codex_linux__ )
#include <codex/reactor/epoll.hpp>
#include <codex/reactor/pipe_wakeup.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/poll.h>
typedef codex::reactor::epoll poller;
#endif

TEST( reactor , event_equal ) {
#if defined ( __codex_linux__ )
  ASSERT_EQ( (int)EPOLLIN , (int)codex::reactor::pollin );
  ASSERT_EQ( (int)EPOLLOUT , (int)codex::reactor::pollout );
  ASSERT_EQ( (int)POLLIN , (int)codex::reactor::pollin );
  ASSERT_EQ( (int)POLLOUT , (int)codex::reactor::pollout );
#endif
}


TEST( reactor , bind ) {
  poller poll;
  int udpfd = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
  ASSERT_TRUE( udpfd != -1 );
  codex::reactor::event_handler handler( nullptr );
  ASSERT_TRUE( poll.bind( udpfd , codex::reactor::pollin , &handler ));
  poll.unbind( udpfd );
  ::close( udpfd );
}

TEST( reactor , pipe_wakeuwakeupp) {
  poller poll;
  codex::reactor::pipe_wakeup wakeup;
  poll.bind( wakeup.handle() , codex::reactor::pollin , wakeup.handler());
  ASSERT_EQ( poll.wait( 100 ) , 0 );
  wakeup.set();
  ASSERT_EQ( poll.wait( 100 ) , 1 );
} 
