/**
 */

#include <codex/reactor/reactor.hpp>

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
    bool bind( int fd 
        , const int events 
        , reactor::event_handler* handler );

    /// 
    void unbind( int fd );

    ///
    int wait( const int wait_ms );
  private:
    int _epoll_fd;
  };

}}
