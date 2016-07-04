/*
 */
#ifndef __codex_io_async_layer_h__
#define __codex_io_async_layer_h__

#if defined( __codex_win32__ )
#include <codex/proactor/async_layer.hpp>
#else
#include <codex/reactor/async_layer.hpp>
#endif

namespace codex { namespace io {

#if defined( __codex_win32__ )
typedef codex::proactor::async_layer async_layer;
#else
typedef codex::reactor::async_layer async_layer;
#endif


}}

#endif
