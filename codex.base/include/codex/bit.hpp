/**
 */
#ifndef __codex_bit_h__
#define __codex_bit_h__
namespace codex {

  template < typename T , typename R = uint32_t >
  constexpr R bit( T idx ){
      return static_cast<R>( 1u << (R)idx );
  }

  /*
   * enum a{
   * flag1 = bit(0) ,
   * flag2 = bit(1) ,
   * };
   *
   * enum class flagidx {
   *     flaga,
   *     flagb,
   *     flagc,
   * };
   * 
   * enum flag{
   *     flaga=bit(flagidx::flaga),
   *     flagb=bit(flagidx::flagb),
   * };
   *
   */

}
#endif
