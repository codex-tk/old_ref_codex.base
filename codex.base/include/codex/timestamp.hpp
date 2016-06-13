#ifndef __codex_timestamp_h__
#define __codex_timestamp_h__

#include <codex/timespan.hpp>

namespace codex {

  class timespan;
  /*!
   *  \class timestamp
   *  \brief 
   *  based on 1970/1/1 00:00:00 
   *  microseconds / ucodex
   */
  class timestamp {
  public:
    //! from tick
    explicit timestamp(const int64_t tick = 0 );
    //! copy construct
    timestamp( const timestamp& rhs );
    //! assign
    timestamp& operator=( const timestamp& rhs );
    //! to tick
    int64_t tick( void ) const;

    bool operator==( const timestamp& rhs ) const;
    bool operator!=( const timestamp& rhs ) const;
    bool operator> ( const timestamp& rhs ) const;
    bool operator>=( const timestamp& rhs ) const;
    bool operator< ( const timestamp& rhs ) const;
    bool operator<=( const timestamp& rhs ) const;

    timestamp operator+( const timespan& rhs ) const;
    timestamp operator-( const timespan& rhs ) const;
    timestamp& operator+=( const timespan& rhs ) ;
    timestamp& operator-=( const timespan& rhs ) ;

    timespan  operator-( const timestamp& rhs ) const;

    static timestamp now( void );
  private:
    int64_t _tick;
  };



  struct systemtime
#if defined( __codex_win32__ )
    : public SYSTEMTIME
#endif
    {
    public:
      systemtime( void );
      systemtime( const timestamp& ts );
    public:
#if !defined( __codex_win32__ )
      uint16_t wYear;
      uint16_t wMonth;
      uint16_t wDayOfWeek;
      uint16_t wDay;
      uint16_t wHour;
      uint16_t wMinute;
      uint16_t wSecond;
      uint16_t wMilliseconds;
#endif
    };

  struct filetime 
#if defined( __codex_win32__ )
    : public FILETIME
#endif
  {
  public:
    filetime( void );
    filetime( const timestamp& ts );
#if !defined( __codex_win32__ )
    uint32_t dwHighDateTime;
    uint16_t dwLowDateTime;
#endif
  };

}
#endif
