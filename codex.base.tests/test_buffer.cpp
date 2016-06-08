#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/loop.hpp>
#include <codex/buffer/ctrl_blk.hpp>


TEST( buffer , ctrl_blk ){
  codex::buffer::ctrl_blk* blk = codex::buffer::make_blk(16);
  blk->add_ref();
  ASSERT_EQ( blk->ref_count() , 2 );  
  blk->release();
  ASSERT_EQ( blk->ref_count() , 1 );  
  ASSERT_EQ( blk->release() , 1 );  


  int test_value = 0;
  codex::buffer::ctrl_blk* blkwithdeleter = codex::buffer::make_blk( nullptr , 0  , [&](void* ptr) {
        test_value = 1;
      });

  blkwithdeleter->add_ref();
  ASSERT_EQ( blkwithdeleter->ref_count() , 2 );  
  blkwithdeleter->release();
  ASSERT_EQ( test_value  , 0 );
  ASSERT_EQ( blkwithdeleter->ref_count() , 1 );  
  ASSERT_EQ( blkwithdeleter->release() , 1 );
  ASSERT_EQ( test_value  , 1 );
}
