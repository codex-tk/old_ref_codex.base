#include <gtest/gtest.h>
#if defined ( __codex_linux__ )
#include <codex/codex.hpp>
#include <codex/reactor/reactor.hpp>
#include <codex/pipe.hpp>


#include <codex/reactor/epoll.hpp>
#include <codex/reactor/pipe_wakeup.hpp>
#include <codex/reactor/eventfd_wakeup.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/poll.h>
typedef codex::reactor::epoll poller;
typedef codex::reactor::eventfd_wakeup wakeup;


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
  codex::reactor::poll_handler handler( nullptr );
  handler.events( codex::reactor::pollin );
  ASSERT_TRUE( poll.bind( udpfd , &handler ));
  poll.unbind( udpfd );
  ::close( udpfd );
}

TEST( reactor , wakeup ) {
  poller poll;
  wakeup wakeup;
  poll.bind( wakeup.handle() , wakeup.handler());
  ASSERT_EQ( poll.wait( 100 ) , 0 );
  wakeup.set();
  ASSERT_EQ( poll.wait( 100 ) , 1 );
  ASSERT_EQ( poll.wait( 100 ) , 0 );
  poll.unbind( wakeup.handle() );
} 
#endif