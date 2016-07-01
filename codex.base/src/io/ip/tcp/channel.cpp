#include <codex/io/ip/tcp/channel.hpp> 

namespace codex { namespace io {  namespace ip {  namespace tcp {
  event_handler::event_handler(void) {

  }

  event_handler::~event_handler(void) {
  }

  void event_handler::channel_ptr(tcp::channel_ptr ptr) {
    _channel_ptr = ptr;
  }

  tcp::channel_ptr& event_handler::channel_ptr(void) {
    return _channel_ptr;
  }

  void event_handler::on_error0(const std::error_code& ec) {
    on_error(ec);
    _channel_ptr.reset();
  }
}}}}
