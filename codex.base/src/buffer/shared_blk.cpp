#include <codex/buffer/shared_blk.hpp>

namespace codex { namespace buffer {
 
  shared_blk::shared_blk( void ) 
    : _blk( nullptr )
  {
  }

  shared_blk::shared_blk( const shared_blk& rhs )
    : _blk( rhs._blk ) , _read( rhs._read ) 
    , _write( rhs._write )
  {
    add_ref();
  }

  shared_blk::shared_blk( shared_blk&& rhs )
    : _blk( rhs._blk ) , _read( rhs._read ) 
    , _write( rhs._write )
  {
    rhs._blk = nullptr;
    rhs._read = rhs._write = 0;
  }

  shared_blk::shared_blk( const std::size_t sz)
    : _blk( codex::buffer::make_blk(sz))
    , _read(0) , _write(0)
  {
  }

  shared_blk::shared_blk( void* buf , const std::size_t sz )
    : _blk( codex::buffer::make_blk( buf , sz ) )
  {
  }

  shared_blk& shared_blk::operator=( const shared_blk& rhs ){
    shared_blk blk(rhs);
    swap(blk);
    return *this;
  }

  shared_blk& shared_blk::operator=( shared_blk&& rhs ) {
    shared_blk blk( std::move( rhs ) );
    swap( blk );
    return *this;
  }

  shared_blk::~shared_blk( void ) {
    release();
  }

  void shared_blk::add_ref( void ) {
    if ( _blk ) _blk->add_ref();
  }

  void shared_blk::release( void ) {
    if ( _blk ) _blk->release();
  }

  int shared_blk::ref_count( void ) {
    if ( _blk ) return _blk->ref_count();
    return 0;
  }

  void shared_blk::swap( shared_blk& rhs ) {
    std::swap( _blk , rhs._blk );
    std::swap( _read , rhs._read );
    std::swap( _write, rhs._write );
  }

  std::size_t shared_blk::size( void ) const {
    if ( _blk ) return _blk->size();
    return 0;
  }
  
  void* shared_blk::read_ptr( void ) {
    if ( _blk == nullptr )
      return nullptr;
    uint8_t* ptr = static_cast< uint8_t* >(_blk->ptr());
    return ptr + _read;
  }

  int shared_blk::read_ptr( const int m ) {
    int move = m;
    int read = static_cast<int>(_read); 
    read += move;
    if ( read < 0 ) {
      move -= read;
      read = 0;
    }
    if ( read > static_cast<int>(_write) ) {
      move -= ( read - _write );
      read = _write;
    }
    _read = read;
    return move;
  }

  std::size_t shared_blk::length( void ) const {
    return _write - _read;
  }
 
  void* shared_blk::write_ptr( void ) {
    if ( _blk == nullptr )
      return nullptr;
    uint8_t* ptr = static_cast< uint8_t* >(_blk->ptr());
    return ptr + _write;
  }

  int shared_blk::write_ptr( const int m ) {
    int move = m;
    int write = static_cast<int>(_write); 
    write += move;
    if ( write < static_cast<int>(_read) ) {
      move += ( _read - write );
      write = _read;
    }
    int sz = static_cast<int>(size());
    if ( write > sz ) {
      move -= ( write - sz);
      write = sz;
    }
    _write = write;
    return move;
  }

  std::size_t shared_blk::space( void ) const {
    return size() - _write;
  }


  void shared_blk::clear( void ) {
    _read = _write = 0;
  }

}}
