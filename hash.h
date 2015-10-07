/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2015 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |                                                                     
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include<iostream>
#include<stdint.h>

#define ALWAYS_INLINE inline
namespace MurmurHash3 {
///////////////////////////////////////////////////////////////////////////////
// The following code is based on MurmurHash3:
//   http://code.google.com/p/smhasher/wiki/MurmurHash3
//
// The case-insensitive version converts lowercase characters to uppercase
// under the assumption that character data are 7-bit ASCII. This should work
// as identifiers usually only contain alphanumeric characters and the
// underscore. Although PHP allows higher ASCII characters (> 127) in an
// identifier, they should be very rare, and do not change the correctness.

#define ROTL64(x,y) rotl64(x,y)
#define BIG_CONSTANT(x) (x##LLU)

ALWAYS_INLINE uint64_t rotl64(uint64_t x, int8_t r) {
  return (x << r) | (x >> (64 - r));
}

template <bool caseSensitive>
ALWAYS_INLINE uint64_t getblock64(const uint64_t *p, int i) {
  uint64_t block = p[i];
  if (!caseSensitive) {
    block &= 0xdfdfdfdfdfdfdfdfLLU; // a-z => A-Z
  }
  return block;
}

template <bool caseSensitive>
ALWAYS_INLINE uint8_t getblock8(const uint8_t *p, int i) {
  uint8_t block = p[i];
  if (!caseSensitive) {
    block &= 0xdfU; // a-z => A-Z
  }
  return block;
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche
ALWAYS_INLINE uint64_t fmix64(uint64_t k) {
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;
  return k;
}

// Optimized for 64-bit architectures.  MurmurHash3 also implements a 128-bit
// hash that is optimized for 32-bit architectures (omitted here).
template <bool caseSensitive>
ALWAYS_INLINE void hash128(const void *key, size_t len, uint64_t seed,
                           uint64_t out[2]) {
  const uint8_t *data = (const uint8_t *)key;
  const size_t nblocks = len / 16;
  uint64_t h1 = seed;
  uint64_t h2 = seed;
  const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
  const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

  //----------
  // body
  const uint64_t *blocks = (const uint64_t *)(data);
  for(size_t i = 0; i < nblocks; i++)
  {
    uint64_t k1 = getblock64<caseSensitive>(blocks,i*2+0);
    uint64_t k2 = getblock64<caseSensitive>(blocks,i*2+1);
    k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
    h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;
    k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;
    h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
  }

  //----------
  // tail
  const uint8_t *tail = (const uint8_t*)(data + nblocks*16);
  uint64_t k1 = 0;
  uint64_t k2 = 0;
  switch(len & 15)
  {
  case 15: k2 ^= uint64_t(getblock8<caseSensitive>(tail, 14)) << 48;
  case 14: k2 ^= uint64_t(getblock8<caseSensitive>(tail, 13)) << 40;
  case 13: k2 ^= uint64_t(getblock8<caseSensitive>(tail, 12)) << 32;
  case 12: k2 ^= uint64_t(getblock8<caseSensitive>(tail, 11)) << 24;
  case 11: k2 ^= uint64_t(getblock8<caseSensitive>(tail, 10)) << 16;
  case 10: k2 ^= uint64_t(getblock8<caseSensitive>(tail,  9)) << 8;
  case  9: k2 ^= uint64_t(getblock8<caseSensitive>(tail,  8)) << 0;
           k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

  case  8: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  7)) << 56;
  case  7: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  6)) << 48;
  case  6: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  5)) << 40;
  case  5: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  4)) << 32;
  case  4: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  3)) << 24;
  case  3: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  2)) << 16;
  case  2: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  1)) << 8;
  case  1: k1 ^= uint64_t(getblock8<caseSensitive>(tail,  0)) << 0;
           k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization
  h1 ^= len; h2 ^= len;
  h1 += h2;
  h2 += h1;
  h1 = fmix64(h1);
  h2 = fmix64(h2);
  h1 += h2;
  h2 += h1;

  ((uint64_t*)out)[0] = h1;
  ((uint64_t*)out)[1] = h2;
}

#undef ROTL64
#undef BIG_CONSTANT
///////////////////////////////////////////////////////////////////////////////
} // namespace MurmurHash3

