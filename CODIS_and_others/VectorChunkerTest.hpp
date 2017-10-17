#ifndef __VECTOR_CHUNKER_TEST__
#define __VECTOR_CHUNKER_TEST__

#include <stdio.h>


#include "VectorChunker.h"
#include "utilities.h"

using std::printf;

void testVectorChunker() {
	vector<Int> vlist;
	dataGen(vlist, 100, 2);
	VectorChunker chunker(vlist);
	
	printIntList(vlist);
	
	WordDescriptor wd;

	wd = chunker.next();
	while(wd.type != WordDescriptor::END) {
		printf("\n%lx", wd.word);
		wd = chunker.next();
	}

	printf("\n");
}


#endif