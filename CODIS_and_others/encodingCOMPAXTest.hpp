#ifndef __ENCODING_COMPAX_TEST__
#define __ENCODING_COMPAX_TEST__

#include "COMPAXSet.h"
#include "utilities.h"

namespace COMPAXSpace{

void setBitmap(Bitmap& bitmap) {
	bitmap.push_back(0x40000001);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x80000001);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x80070080);
	bitmap.push_back(0x40000001);
}

void testFromToVector(){
	vector<Int> data;
	Bitmap bitmap;
	setBitmap(bitmap);
	dataGenFromWAH(data, bitmap);

	COMPAXSet codis;

	codis.fromVector(data);
	printf("original: \n");
	printBitmap(bitmap);
	
	printBitmap(codis.bitmap);

	vector<Int> result;
	codis.toVector(result);
	if (compareIntList(data, result)) {
		printf("check over\n");
	}
	else {
		printf("check failed\n");
	}
	printf("\n\n");
}

};

#endif
