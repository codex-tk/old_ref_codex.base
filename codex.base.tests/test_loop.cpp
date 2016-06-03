#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/loop.hpp>

TEST( loop , post ) {
  codex::loop loop;
  loop.dispatch();

  int value = 0;
  loop.post( codex::loop::operation_type::wrap( [&] {
        value = 1; 
      }));
  ASSERT_EQ( value , 0 );
  loop.dispatch();
  ASSERT_EQ( value , 1 );
}
