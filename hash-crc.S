#include "etch-helpers.h"
#if defined(__powerpc64__)

.file   "hash-crc.S"

.section ".toc"

ETCH_LABEL(C0):
.double 0xdfdfdfdfdfdfdfdf

ETCH_SECTION(hash_string_i_unaligned_crc)
.globl  ETCH_NAME(hash_string_i_unaligned_crc)
ETCH_TYPE(ETCH_NAME(hash_string_i_unaligned_crc), @function)
ETCH_ALIGN16

ETCH_NAME(hash_string_i_unaligned_crc):
	CFI(startproc)
	// r3 = arKey
	// r4 = keyLen

	xor 2, 2, 2
	xor 10, 10, 2

	ld 10, .toc@C0

	li 9, 0
CFI(endproc)


#endif

