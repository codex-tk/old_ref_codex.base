/**
 */
#ifndef __codex_reactor_kqueue_h__
#define __codex_reactor_kqueue_h__

#include <codex/reactor/poll_handler.hpp>

namespace codex { namespace reactor{

  /**
   */
  class kqueue{
  public:
    ///
    kqueue(void);
    ///
    ~kqueue(void);

    ///
    int bind( int fd 
        , reactor::poll_handler* handler );

    /// 
    void unbind( int fd );

    ///
    int wait( const int wait_ms );
  private:
    int _kqueue_fd;
  };

}}

#endif
