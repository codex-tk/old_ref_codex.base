#include <gtest/gtest.h>
#include <codex/codex.hpp>

TEST( array , size ) {
  std::array<int,5> arr{ 1 , 2  , 3 };
  ASSERT_EQ( arr.size() , 5 );
}
