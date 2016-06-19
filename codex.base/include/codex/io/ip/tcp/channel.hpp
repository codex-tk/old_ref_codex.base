/**
*/
#ifndef __codex_io_ip_tcp_channel_h__
#define __codex_io_ip_tcp_channel_h__

#include <codex/codex.hpp>

#if defined( __codex_win32__ )
#include <codex/io/ip/tcp/proactor_channel.hpp>
#include <codex/io/ip/tcp/proactor_channel_builder.hpp>
#else
#include <codex/io/ip/tcp/reactor_channel.hpp>
#include <codex/io/ip/tcp/reactor_channel_builder.hpp>
#endif
namespace codex { namespace io { namespace ip { namespace tcp {


#if defined( __codex_win32__ )
typedef proactor_channel channel;
typedef proactor_channel_builder channel_builder;
#else
typedef reactor_channel channel;
typedef reactor_channel_builder channel_builder;
#endif

}}}}
#endif
