#ifndef __codex_allocator_h__
#define __codex_allocator_h__

#include <codex/codex.hpp>

namespace codex {

  class allocator {
  public:
    typedef void* (*alloc_handler)(std::size_t);
    typedef void* (*realloc_handler)( void* , std::size_t );
    typedef void  (*free_handler)( void* );
  public:
    allocator( void );
    ~allocator( void );

    void* alloc( std::size_t sz );
    void* realloc( void* p , std::size_t sz );
    void  free( void* p );
  private:
    alloc_handler _alloc;
    realloc_handler _realloc;
    free_handler _free;
  };

}

#endif
