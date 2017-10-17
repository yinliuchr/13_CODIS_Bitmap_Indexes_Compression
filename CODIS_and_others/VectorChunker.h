#ifndef __VECTOR_CHUNKER__
#define __VECTOR_CHUNKER__

#include "utilities.h"

class VectorChunker{
public:
	const vector<Int>* integers;
	vector<Int>::const_iterator itr;
	Int pointer;

	VectorChunker(const vector<Int>& intList): integers(&intList), itr(intList.begin()), pointer(0) {}
	~VectorChunker() { integers = nullptr; pointer = 0;}

	WordDescriptor next();
};



#endif