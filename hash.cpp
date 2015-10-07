#include<iostream>
#include<stdint.h>
#include "hash.h"

typedef int32_t strhash_t;
const strhash_t STRHASH_MASK = 0x7fffffff;
const strhash_t STRHASH_MSB  = 0x80000000; 

strhash_t hash_string_i_fallback(const char *arKey, uint32_t nKeyLength) {
  uint64_t h[2];
  MurmurHash3::hash128<false>(arKey, nKeyLength, 0, h);
  std::cout << "Output: " << std::hex << h[0] << std::endl;
  return strhash_t(h[0] & STRHASH_MASK);
}


int main(int argc, char **argv){
	strhash_t foo;
	foo = hash_string_i_fallback("foobar", 4);
	std::cout << "int32_t value: " << std::hex << foo << std::endl;

	return 0;
}
