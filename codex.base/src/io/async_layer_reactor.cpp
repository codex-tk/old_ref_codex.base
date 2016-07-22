#include <codex/io/async_layer.hpp>
#include <codex/codex.hpp>
#include <codex/slist.hpp>
#include <codex/operation.hpp>
#include <codex/loop.hpp>
#include <codex/reactor/reactor.hpp>
#include <codex/io/operation.hpp>
#include <codex/io/ip/socket_ops.hpp>

namespace codex{ namespace io {

  struct async_layer::_descriptor{
    codex::loop& loop; 
    int fd;
    reactor::poll_handler handler;

    //codex::slist< codex::io::operation > ops[2];

    _descriptor( codex::loop& l )
      : loop(l)
      , fd(-1)
      , handler( &async_layer::_descriptor::handle_events )
    {
    }

    ~_descriptor( void ) {
      if ( fd != -1 ) 
        ::close(fd);
      fd = -1;
    }

    void handle_events( const int poll_ev ) {
      /*
      static const codex::reactor::poll_events ev[2] = {
        codex::reactor::poll_events::pollin ,
        codex::reactor::poll_events::pollout 
      };
      int remain_ev = 0;
      for ( int i = 0 ; i < 2 ; ++i ) {
        if ( poll_ev & ev[i] ) {
          io::operation* op = ops[i].head();
          while( op ) {
            //if ( !(*op)( layer )) 
            //  break;
            ops[i].remove_head();
            (*op)();
            op = ops[i].head();
          }
        }
        if ( ops[i].head() ){
          remain_ev |= ev[i];
        }
      }
      if ( handler.events() != remain_ev ) {
        handler.events(remain_ev);
        loop.engine().implementation().bind( fd , &handler );
      }
      */
    }

    static void handle_events( reactor::poll_handler* h , const int poll_ev ) {
      _descriptor* desc = codex::container_of( h , &_descriptor::handler );
      desc->handle_events( poll_ev );
    }
  };

  async_layer::descriptor_type async_layer::open( codex::loop& l ){
    return std::make_shared< async_layer::_descriptor >(l);
  }

}}
