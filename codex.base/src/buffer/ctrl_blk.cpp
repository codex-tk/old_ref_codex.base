#include <codex/buffer/ctrl_blk.hpp>

namespace codex { namespace buffer {
  
  ctrl_blk::ctrl_blk( void* p , const std::size_t sz ) 
    : _ref_count(1) , _ptr(p) , _size(sz)
  {
  }
  
  ctrl_blk::~ctrl_blk( void ) {
    
  }

  int ctrl_blk::add_ref( void ) {
    return _ref_count.fetch_add(1);
  }

  int ctrl_blk::release( void ) {
    int val = _ref_count.fetch_sub(1);
    if ( val == 1 ) {
      _on_destroy();
    }
    return val;
  }

  int ctrl_blk::ref_count( void ) {
    return _ref_count.load();
  }

  void* ctrl_blk::ptr( void ) {
    return _ptr;
  }

  std::size_t ctrl_blk::size( void ) {
    return _size;
  }

  void ctrl_blk::_on_destroy( void ) {
    void* ptr = reinterpret_cast< ctrl_blk* >(_ptr) - 1; 
    std::free( ptr );
  }

  ctrl_blk* make_blk( const std::size_t sz ) {
    ctrl_blk* blk= reinterpret_cast< ctrl_blk*>(std::malloc( sizeof( ctrl_blk ) + sz ));
    new (blk) ctrl_blk( blk + 1 , sz );
    return blk;
  }

  ctrl_blk* make_blk( void* p , const std::size_t sz ){
    struct null_deleter {
      void operator()( void* p ) {
      }
    };
    return make_blk( p , sz , null_deleter());
  }

}}
