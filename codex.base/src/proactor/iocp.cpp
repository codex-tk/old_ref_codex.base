#include <codex/proactor/iocp.hpp>

namespace codex { namespace proactor {

  iocp::handler::handler( iocp::handler::callback cb )
    : _callback( cb )
  {
    reset();
  }

  void iocp::handler::operator()( void* ctx 
      , const std::error_code& ec 
      , const int io_bytes )
  {
    if ( _callback ){
      _callback( ctx , ec , io_bytes );
    }
  }

  void iocp::handler::reset( void ) {
    memset( this , 0x00 , sizeof( OVERLAPPED ));
  }

  iocp::iocp( void )
    : _iocp( CreateIoCompletionPort( INVALID_HANDLE_VALUE 
          , NULL , NULL , 0 ))
  {

  }

  iocp::~iocp( void ){
    ::CloseHandle( _iocp );
    _iocp = INVALID_HANDLE_VALUE;
  }


  int iocp::bind( SOCKET fd 
      , void* ctx )
  {
    HANDLE h = CreateIoCompletionPort(reinterpret_cast<HANDLE>(fd)
      , _iocp
      , reinterpret_cast<ULONG_PTR>(ctx)
      , 0);
    if (h == _iocp) {
      return 0;
    }
    return -1;
  }

  void iocp::unbind( SOCKET ){
  }

  void iocp::wakeup(void) {
    ::PostQueuedCompletionStatus(_iocp, -1, 0, nullptr);
  }

  int iocp::wait( const int wait_ms ){
    LPOVERLAPPED overlapped = nullptr;
    ULONG_PTR key = 0;
    DWORD bytes_transferred = 0;
    bool result = GetQueuedCompletionStatus( 
        _iocp
        , &bytes_transferred 
        , &key
        , &overlapped
        , static_cast<DWORD>(wait_ms)) == TRUE;

    if ( overlapped == nullptr ) {
      if (bytes_transferred == -1)
        return 1;
      return 0;
    }
    iocp::handler* handler_ptr= 
      static_cast< iocp::handler* >(overlapped);
    std::error_code ec;
    if ( !result ) {
      ec = std::error_code(GetLastError(), std::system_category());
    }
    (*handler_ptr)( reinterpret_cast<void*>(key) , ec , static_cast<int>(bytes_transferred) );
    return 1;
  }

}}

