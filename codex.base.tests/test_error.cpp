#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/diag/error.hpp>

TEST( error , code ) {
  std::error_code ec = codex::make_error_code( codex::errc::closed_by_user );
  ASSERT_TRUE( ec == codex::errc::closed_by_user );
}
