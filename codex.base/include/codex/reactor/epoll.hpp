/**
 */
#ifndef __codex_reactor_epoll_h__
#define __codex_reactor_epoll_h__

#include <codex/reactor/poll_handler.hpp>

namespace codex { namespace reactor{

  /**
   */
  class epoll{
  public:
    ///
    epoll(void);
    ///
    ~epoll(void);

    ///
    int bind( int fd 
        , reactor::poll_handler* handler );

    /// 
    void unbind( int fd );

    ///
    int wait( const int wait_ms );
  private:
    int _epoll_fd;
  };

}}

#endif
