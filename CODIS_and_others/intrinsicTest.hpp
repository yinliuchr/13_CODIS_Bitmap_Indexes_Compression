#ifndef __INTRINSICS_TEST__
#define __INTRINSICS_TEST__
#include <intrin.h>
#include "utilities.h"

void testBittest(){
	Word w = 0x80000004;
	if (_bittest((const long*)&w, 31)) {
		printf("0x80000004 has '1' at 31th bit\n");
	}

	if (_bittest((const long*)&w, 2)) {
		printf("0x80000004 has '1' at 2th bit\n");
	}
}

void testBitExtraction() {
	Word w = 0x80010103;

	Word out = _bextr_u32(w, 0, 4);
	printf("0x%08x\n", out);

	out = _bextr_u32(w, 4, 8);
	printf("0x%08x\n", out);
}

void testCount() {
	printf("0x%08x : lzcount: %d\n", 0x00000000, _lzcnt_u32(0));
	printf("0x%08x : tzcount: %d\n", 0x00000000, _tzcnt_u32(0));
}

#endif