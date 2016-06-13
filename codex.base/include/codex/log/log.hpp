#ifndef __codex_log_h__
#define __codex_log_h__

#include <codex/log/record.hpp>
#include <codex/log/writer.hpp>
#include <codex/log/formatter.hpp>
#include <codex/log/logger.hpp>
#include <codex/log/console_writer.hpp>
//#include <codex/log/file_writer.hpp>
//#include <codex/log/serialize_formatter.hpp>

#if defined( __codex_win32__ )

#include <crtdefs.h>
  //! win32 macros

#define LOG_T(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOG_D(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOG_I(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOG_W(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOG_E(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOG_F(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)

#define LOG_DUMP_T(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOG_DUMP_D(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOG_DUMP_I(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOG_DUMP_W(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOG_DUMP_E(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOG_DUMP_F(tag,bzffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)

#define LOGGER_T(l,tag,msg,... ) do{l.write(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_D(l,tag,msg,... ) do{l.write(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_I(l,tag,msg,... ) do{l.write(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_W(l,tag,msg,... ) do{l.write(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_E(l,tag,msg,... ) do{l.write(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_F(l,tag,msg,... ) do{l.write(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__ ); } while(0)
      
#define LOGGER_DUMP_T(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_D(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_I(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_W(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_E(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_F(l,tag,bzffer,sz,msg,... ) do{l.write_dump(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,__VA_ARGS__ ); } while(0)

#else
//! other os macros

#define LOG_T(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOG_D(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOG_I(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOG_W(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOG_E(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOG_F(tag,msg,... ) do{codex::log::logger::instance().write(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)

#define LOG_DUMP_T(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_D(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_I(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_W(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_E(tag,buffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_F(tag,bzffer,sz,msg,... ) do{codex::log::logger::instance().write_dump(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)


#define LOGGER_T(l,tag,msg,... ) do{l.write(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_D(l,tag,msg,... ) do{l.write(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_I(l,tag,msg,... ) do{l.write(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_W(l,tag,msg,... ) do{l.write(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_E(l,tag,msg,... ) do{l.write(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_F(l,tag,msg,... ) do{l.write(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,msg,##__VA_ARGS__ ); } while(0)
        
#define LOGGER_DUMP_T(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::trace,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_D(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::debug,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_I(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::info,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_W(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::warn,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_E(l,tag,buffer,sz,msg,... ) do{l.write_dump(codex::log::error,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_F(l,tag,bzffer,sz,msg,... ) do{l.write_dump(codex::log::fatal,tag,__FILE__,__FUNCTION__,__LINE__,(uint8_t*)buffer,sz,msg,##__VA_ARGS__ ); } while(0)

#endif

#endif
