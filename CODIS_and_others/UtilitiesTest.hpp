#ifndef __UTILITIES_TEST__
#define __UTILITIES_TEST__

#include "utilities.h"

void testDataGenFromWAH(){
	vector<Int> v;
	Bitmap bitmap;

	bitmap.push_back(0xC0007003);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x40000001);

	dataGenFromWAH(v, bitmap);

	printIntList(v);
}




#endif