#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/loop.hpp>

namespace {

class loop_f: public ::testing::Test{
protected:
  virtual void SetUp()
  {      
    loop = new codex::loop();
  }

  virtual void TearDown()
  {
    delete loop;
  }
  codex::loop* loop;
};
}

TEST_F( loop_f , post ){
  loop->dispatch(0);

  int value = 0;
  loop->post( codex::loop::operation_type::wrap( [&] {
        ASSERT_TRUE( loop->in_loop() );
        value = 1; 
      }));
  ASSERT_EQ( value , 0 );
  ASSERT_FALSE( loop->in_loop() );
  loop->dispatch(100);
  ASSERT_FALSE( loop->in_loop() );
  ASSERT_EQ( value , 1 );
}


TEST_F( loop_f , post2 ) {
  loop->post_handler( [] {} );
  ASSERT_EQ( 2 , loop->dispatch(0));
}
