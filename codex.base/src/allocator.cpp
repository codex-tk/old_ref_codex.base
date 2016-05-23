#include <codex/allocator.hpp>

namespace codex {
  allocator::allocator( void )
    : _alloc( &std::malloc )
    , _realloc( &std::realloc )
    , _free( &std::free )
  {
  }

  allocator::~allocator( void ) {

  }

  void* allocator::alloc( std::size_t sz ) {
    return _alloc( sz );
  }
  void* allocator::realloc( void* p , std::size_t sz ) {
    return _realloc( p , sz );
  }
  void  allocator::free( void* p ){
    return _free( p );
  }
}
