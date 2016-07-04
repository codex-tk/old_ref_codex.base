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
    codex::io::ip::socket_ops<>::socket_type fd;
    poll_handler handler;
    // todo change io_operation 
    codex::slist< codex::io::operation > ops[2];

    descriptor() 
      : handler( &async_layer::descriptor::handle_events )
    {
    }

    void handle_events( const int poll_ev ) {
      static const codex::reactor::poll_events ev[2] = {
        codex::reactor::poll_events::pollin ,
        codex::reactor::poll_events::pollout 
      };
      for ( int i = 0 ; i < 2 ; ++i ) {
        if ( poll_ev & ev[i] ) {
          while ( ops[i].head() ){

            break;
          }
        }
      }
   }

    static void handle_events( poll_handler* h , const int poll_ev ) {

    }
  };

  async_layer::async_layer( loop& l , engine& e )
    : _loop(l) , _engine(e)
  {
  }

  async_layer::~async_layer( void ) {

  }

}}
