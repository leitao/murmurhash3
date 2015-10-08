#include "etch-helpers.h"
#if defined(__powerpc64__)

.file   "hash-crc.S"

.section ".toc"

ETCH_LABEL(C0):
	.quad 0xdfdfdfdfdfdfdfdf

ETCH_SECTION(hash_string_i_unaligned_crc)
.globl  ETCH_NAME(hash_string_i_unaligned_crc)
ETCH_TYPE(ETCH_NAME(hash_string_i_unaligned_crc), @function)
ETCH_ALIGN16

ETCH_NAME(hash_string_i_unaligned_crc):
	CFI(startproc)
	// r3 = arKey
	// r4 = keyLen

	// TODO: Create the ppc64le code here
	li 9, 0
CFI(endproc)


#endif

