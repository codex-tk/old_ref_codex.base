#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/allocator.hpp>

TEST( test , allocator ) {
  codex::allocator allocator;
  allocator.free(allocator.alloc(32));
}
