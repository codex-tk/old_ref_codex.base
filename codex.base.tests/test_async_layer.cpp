#include <gtest/gtest.h>
#include <codex/loop.hpp>
#include <codex/io/async_layer.hpp>


TEST( async_layer , descriptor ) {
  codex::loop l;
  codex::io::async_layer::descriptor_type fd = 
    codex::io::async_layer::open( l );
}

