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
  length_packetizer::length_packetizer( void  )
  {
    
  }

  length_packetizer::~length_packetizer( void ){

  }

  int length_packetizer::setup( codex::io::buffer* iobufs , int cnt ) {
    if ( _blk.size() == 0 ) {
      _blk = shared_blk(default_buf_size());
    }
    int hdr_sz = header_size();
    int len = _blk.length();
    if ( len  < hdr_sz ) {
      iobufs[0].length( hdr_sz  - len );
    } else {
      int body_sz = body_size( _blk.read_ptr());
      if ( len + static_cast<int>(_blk.space()) < hdr_sz + body_sz ) {
        _blk = codex::buffer::reserve( _blk , hdr_sz + body_sz );
      }
      iobufs[0].length( hdr_sz + body_sz - len );
    }
    iobufs[0].ptr( static_cast<char*>(_blk.write_ptr()));
    return 1;
  }
  void length_packetizer::assemble( const int readbytes ) {
    _blk.write_ptr(readbytes);
  }

  bool length_packetizer::done( void ) {
    int hdr_sz = header_size();
    int len = _blk.length();
    if ( len <= hdr_sz ) {
      return false;
    }
    int body_sz = body_size( _blk.read_ptr());
    return len == ( hdr_sz + body_sz ); 
  }

  shared_blk length_packetizer::packet( void ) {
    shared_blk nblk(default_buf_size());
    nblk.swap(_blk);
    return nblk;
  }
  
  void length_packetizer::clear( void ){
    _blk.clear(); 
  }


  int length_packetizer::default_buf_size( void ){
    return 4096;
  }
}}
