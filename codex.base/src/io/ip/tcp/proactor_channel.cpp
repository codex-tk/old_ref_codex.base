#include <codex/io/ip/tcp/proactor_channel.hpp>
#include <codex/loop.hpp>
#include <codex/log/log.hpp>
#include <codex/diag/error.hpp>
#include <codex/io/ip/tcp/proactor_channel_builder.hpp>

namespace codex { namespace io {  namespace ip {  namespace tcp {

  namespace {
    int k_handle_error_bit = 0x10000000;
    int k_handle_close_bit = 0x20000000;
  }

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

  proactor_channel::proactor_channel(void)
    : _fd(ip::socket_ops<>::invalid())
    , _read_handler(&proactor_channel::handle_read0)
    , _write_handler(&proactor_channel::handle_write0)
    , _ref_count(k_handle_close_bit | k_handle_error_bit | 1 )
    , _loop(nullptr)
    , _builder(nullptr)
  {
  }

  proactor_channel::~proactor_channel(void) {

  }

  codex::loop& proactor_channel::loop(void) {
    return *_loop;
  }

  int proactor_channel::bind(int fd) {
    _fd = fd;
    codex::io::ip::socket_ops<int>::nonblocking(_fd);
    if (_loop->engine().impl().bind(_fd, this) < 0)
      return -1;
    do_read();
    return 0;
  }

  void proactor_channel::close(void) {
    if ( _ref_count.load() & k_handle_close_bit) {
      do {
        int expected = _ref_count.load();
        int desired = expected & ~k_handle_close_bit;
        if ( _ref_count.compare_exchange_strong(expected,desired))
          break;
      } while(true);
      add_ref();
      _loop->post_handler( [this]{
          handle_error( codex::make_error_code( codex::errc::closed_by_user ));
          release();
      });
    }
  }

  bool proactor_channel::closed(void) {
    return (_ref_count.load() & k_handle_error_bit) == 0;
  }

  void proactor_channel::write(codex::buffer::shared_blk blk) {
    if (blk.length() <= 0)
      return;
    add_ref();
    _loop->post_handler([this, blk] {
      write0(blk);
      release();
    });
  }

  int proactor_channel::add_ref(void) {
    return _ref_count.fetch_add(1);
  }

  int proactor_channel::release(void) {
    int cnt = _ref_count.fetch_sub(1);
    if (cnt == 1) {
      _builder->on_end_reference(this);
    }
    return cnt;
  }

  void proactor_channel::reset(void) {
    _ref_count = k_handle_close_bit | k_handle_error_bit | 1;
    _fd = ip::socket_ops<>::invalid();
    _write_packets.clear();
    _packetizer.reset();
  }

  void proactor_channel::set_builder(proactor_channel_builder* builder) {
    _builder = builder;
  }

  void proactor_channel::set_loop(codex::loop* loop) {
    _loop = loop;
  }

  void proactor_channel::set_packetizer(
    const std::shared_ptr< codex::buffer::packetizer >& packetizer) {
    _packetizer = packetizer;
  }

  void proactor_channel::set_handler(const std::shared_ptr< event_handler >& handler) {
    _handler = handler;
  }

  void proactor_channel::handle_read(const std::error_code& ec
    , const int io_bytes) {
    if ((_ref_count.load() & k_handle_close_bit) == 0) {
      return;
    }
    if (ec) {
      return handle_error(ec);
    }

    if (io_bytes <= 0) {
      return handle_error(codex::make_error_code(codex::errc::disconnect));
    }

    _packetizer->assemble(io_bytes);

    while (_packetizer->done()) {
      auto blk = _packetizer->packet();
      if (_handler) {
        _handler->on_read(blk);
      }
    }
    do_read();
  }

  void proactor_channel::handle_write(const std::error_code& ec
    , const int io_bytes) {
    if ((_ref_count.load() & k_handle_close_bit) == 0) {
      return;
    }
    if (ec) {
      return handle_error(ec);
    }
    if (io_bytes <= 0) {
      return handle_error(codex::make_error_code(codex::errc::disconnect));
    }

    int writebytes = io_bytes;
    while (writebytes > 0) {
      writebytes -= _write_packets.front().read_skip(writebytes);
      if (_write_packets.front().length() == 0) {
        _write_packets.pop_front();
      }
    }
    do_write();
    if (_handler) {
      _handler->on_write(io_bytes, _write_packets.empty());
    }
  }

  void proactor_channel::write0(codex::buffer::shared_blk blk) {
    bool req = _write_packets.empty();
    _write_packets.push_back(blk);
    if (req) {
      do_write();
    }
  }

  void proactor_channel::handle_error(const std::error_code& ec) {
    if (_ref_count.load() & k_handle_error_bit) {
      do {
        int expected = _ref_count.load();
        int desired = expected & ~k_handle_error_bit;
        if (_ref_count.compare_exchange_strong(expected, desired))
          break;
      } while (true);
      _loop->engine().impl().unbind(_fd);
      ip::socket_ops<>::close(_fd);
      if (_handler) {
        _handler->on_error0(ec);
      }
    }
    if (ec == codex::errc::closed_by_user) {
      release();
    }
  }

  void proactor_channel::do_read(void) {
    codex::io::buffer buf[32];
    int iovcnt = _packetizer->setup(buf, 32);

    _read_handler.reset();

    add_ref();

    DWORD flag = 0;
    if (WSARecv(_fd
      , buf
      , iovcnt
      , nullptr
      , &flag
      , &_read_handler
      , nullptr) == SOCKET_ERROR)
    {
      std::error_code ec(WSAGetLastError(), std::system_category());
      if (ec.value() != WSA_IO_PENDING) {
        _loop->post_handler([this, ec] {
          handle_write(ec, 0);
        });
        return;
      }
    }
  }

  void proactor_channel::do_write(void) {
    if (_write_packets.empty())
      return;
    int iovcnt = std::min(static_cast<int>(_write_packets.size()), 32);
    codex::io::buffer buf[32];
    for (int i = 0; i < iovcnt; ++i) {
      buf[i].ptr(static_cast<char*>(_write_packets[i].read_ptr()));
      buf[i].length(static_cast<int>(_write_packets[i].length()));
    }

    _write_handler.reset();

    add_ref();

    DWORD flag = 0;
    if (WSASend(_fd
      , buf
      , iovcnt
      , nullptr
      , flag
      , &_write_handler
      , nullptr) == SOCKET_ERROR)
    {
      std::error_code ec(WSAGetLastError(), std::system_category());
      if (ec.value() != WSA_IO_PENDING) {
        _loop->post_handler([this, ec] {
          handle_write(ec, 0);
        });
        return;
      }
    }
  }

  void proactor_channel::handle_read0(void* ctx
    , const std::error_code& ec
    , const int io_bytes)
  {
    proactor_channel* chan = static_cast<proactor_channel*>(ctx);
    chan->handle_read(ec, io_bytes);
    chan->release();
  }

  void proactor_channel::handle_write0(void* ctx
    , const std::error_code& ec
    , const int io_bytes)
  {
    proactor_channel* chan = static_cast<proactor_channel*>(ctx);
    chan->handle_write(ec, io_bytes);
    chan->release();
  }

}}}}
