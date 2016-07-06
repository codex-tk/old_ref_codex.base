#include <codex/codex.hpp>
#include <codex/slist.hpp>
#include <codex/operation.hpp>
#include <codex/loop.hpp>
#include <codex/reactor/reactor.hpp>
#include <codex/reactor/async_layer.hpp>
#include <codex/io/operation.hpp>
#include <codex/io/ip/socket_ops.hpp>

namespace codex { namespace reactor {

  struct async_layer::descriptor {
    int fd;
    poll_handler handler;
    async_layer* layer;
    codex::slist< codex::io::operation > ops[2];

    descriptor( async_layer* l , int native_fd ) 
      : fd ( native_fd )
      , handler( &async_layer::descriptor::handle_events )
      , layer(l)
    {
    }

    ~descriptor( void ) {

    }

    void handle_events( const int poll_ev ) {
      static const codex::reactor::poll_events ev[2] = {
        codex::reactor::poll_events::pollin ,
        codex::reactor::poll_events::pollout 
      };
      for ( int i = 0 ; i < 2 ; ++i ) {
        if ( poll_ev & ev[i] ) {
          io::operation* op = ops[i].head();
          while( op ) {
            if ( !(*op)( layer )) 
              break;
            ops[i].remove_head();
            (*op)();
            op = ops[i].head();
          }
        }
      }
    }

    static void handle_events( poll_handler* h , const int poll_ev ) {
      descriptor* desc = codex::container_of( h , &descriptor::handler );
      desc->handle_events( poll_ev );
    }
  };

  async_layer::async_layer( loop& l , engine& e )
    : _loop(l) , _engine(e)
  {
  }

  async_layer::~async_layer( void ) {

  }

  async_layer::descriptor_type async_layer::wrap( int native_fd ) {
    return std::make_shared< descriptor >(this,native_fd);
  }
}}
