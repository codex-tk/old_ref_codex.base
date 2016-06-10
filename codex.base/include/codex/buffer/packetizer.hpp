/**
 */
#ifndef __codex_buffer_packetizer_h__
#define __codex_buffer_packetizer_h__

#include <codex/buffer/shared_blk.hpp>

#include <codex/io/buffer.hpp>

namespace codex { namespace buffer {


  /**
   * packet assembler
   *
   * ...
   * on_readable
   * ...
   *
   * io::buffer bufs[64];
   * int bufcnt = packetizer->setup( bufs , 64 );
   * int len = readv( ... , bufs , bufcnt );
   * if ( len > 0 ){
   *  packetizer->assemble(len);
   *  while( packetizer->done() ) {
   *    shared_blk pkt = packetizer->packet();
   *    do_some_thing_with( pkt );
   *  }
   * } else {
   *   // connection closed or occur errors
   *   // remove old bytes
   *   packetizer->clear();
   * }
   */
  class packetizer{
  public:
    packetizer( void );
    virtual ~packetizer( void );

    virtual int setup( codex::io::buffer* iobufs , int cnt ) = 0;
    virtual void assemble( const int readbytes ) = 0;
    virtual bool done( void ) = 0; 
    virtual shared_blk packet( void ) = 0;
    virtual void clear( void )= 0;
  };

  class random_packetizer : public packetizer  {
  public:
    random_packetizer( const std::size_t buf_size = 4096 );
    virtual ~random_packetizer( void );
    virtual int setup( codex::io::buffer* iobufs , int cnt );
    virtual void assemble( const int readbytes );
    virtual bool done( void ); 
    virtual shared_blk packet( void );
    virtual void clear( void );
  private:
    shared_blk _blk;
  };

}}

#endif
