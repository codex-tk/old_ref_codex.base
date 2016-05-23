#include <gtest/gtest.h>
#include <codex/codex.hpp>

TEST( test , codex_base ) {
	ASSERT_EQ( codex::codex0() , 81 );
}
