#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/loop.hpp>
#include <codex/buffer/ctrl_blk.hpp>
#include <codex/buffer/shared_blk.hpp>


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

TEST( shared_blk ,  basic ) {
  codex::buffer::shared_blk blk;
  ASSERT_EQ( blk.ref_count() , 0 );
  ASSERT_EQ( (int)blk.size() , 0 );
}

TEST( shared_blk , ownership ){
  codex::buffer::shared_blk blk2( 32 );
  ASSERT_EQ( (int)blk2.size() , 32 );
  ASSERT_NE( blk2.read_ptr() , nullptr );
  ASSERT_NE( blk2.write_ptr() , nullptr );
  ASSERT_EQ( blk2.ref_count() , 1 );
  {
    codex::buffer::shared_blk bls3( blk2 );
    ASSERT_EQ( blk2.ref_count() , 2 );
  }
  ASSERT_EQ( blk2.ref_count() , 1 );

  codex::buffer::shared_blk blk( std::move( blk2 ));
  ASSERT_EQ( blk2.ref_count() , 0 );
  ASSERT_EQ( blk.ref_count() , 1 );

  blk2 = blk;
  ASSERT_EQ( blk.ref_count() , 2 );
  ASSERT_EQ( blk2.ref_count() , 2 );

  blk = std::move( blk2 );
  ASSERT_EQ( blk2.ref_count() , 0 );
  ASSERT_EQ( blk.ref_count() , 1 );
}

TEST( shared_blk , read_write ) {
  codex::buffer::shared_blk blk(32);

  // 0 - read(0) - write(0) - size(32)  
  ASSERT_EQ( blk.length()  , 0 );
  ASSERT_EQ( blk.space() , 32 );
  ASSERT_EQ( blk.size() , 32 );

  ASSERT_EQ( blk.read_ptr(-1) , 0 );
  ASSERT_EQ( blk.read_ptr(1), 0 );

  // 0 - read(0) - write(28) - size(32)  
  ASSERT_EQ( blk.write_ptr(-1) , 0 );
  ASSERT_EQ( blk.write_ptr(33) , 32 );
  ASSERT_EQ( blk.write_ptr(-4) , -4 );
  ASSERT_EQ( blk.length() , 28 );

  // 0 - read(20) - write(28) - size(32)  
  ASSERT_EQ( blk.read_ptr( 20 ) , 20 );
  ASSERT_EQ( blk.length() , 8 );
  
  // 0 - read(20) - write(20) - size(32)  
  ASSERT_EQ( blk.write_ptr( -10 ) , -8 );
  ASSERT_EQ( blk.length() , 0 );

  // 0 - read(20) - write(28) - size(32)  
  ASSERT_EQ( blk.write_ptr( 8 ) , 8 );

  // 0 - read(0) - write(28) - size(32)
  ASSERT_EQ( blk.read_ptr( -40 ) , -20 );
  ASSERT_EQ( blk.length() , 28 );

  // 0 - read(28) - write(28) - size(32)
  ASSERT_EQ( blk.read_ptr( 40 ) , 28 );
  ASSERT_EQ( blk.length() , 0 );

  // 0 - read(0) - write(0) - size(32)  
  blk.clear();
  ASSERT_EQ( blk.length()  , 0 );
  ASSERT_EQ( blk.space() , 32 );
  ASSERT_EQ( blk.size() , 32 );
}
