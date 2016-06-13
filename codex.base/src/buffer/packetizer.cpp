#include <codex/buffer/packetizer.hpp>

namespace codex { namespace buffer {

  packetizer::packetizer( void ){
  }

  packetizer::~packetizer( void ) {
  }

  random_packetizer::random_packetizer( const std::size_t buf_size )
    : _blk( buf_size )
  {
  }
  
  random_packetizer::~random_packetizer( void ){
  }

  int random_packetizer::setup( codex::io::buffer* iobufs , const int count ) {
    if ( count <= 0 ) return 0;
    iobufs[0].ptr( static_cast<char*>(_blk.write_ptr()));
    iobufs[0].length( static_cast<int>(_blk.space()));
    return 1;
  }

  void random_packetizer::assemble( const int readbytes ) {
    if ( readbytes > 0 )
      _blk.write_ptr( readbytes );
  }
  
  bool random_packetizer::done( void ){
    return _blk.length() > 0;
  }

  shared_blk random_packetizer::packet( void ) {
    buffer::shared_blk blk( _blk.size() );
    _blk.swap( blk );
    return blk;
  }

  void random_packetizer::clear( void ) {
    _blk.clear();
  }

}}
