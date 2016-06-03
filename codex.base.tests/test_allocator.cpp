#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/allocator.hpp>

TEST( allocator , alloc_free ) {
  codex::allocator allocator;
  allocator.free(allocator.alloc(32));
}
