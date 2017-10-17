#ifndef __ENCODING_PLWAH_TEST__
#define __ENCODING_PLWAH_TEST__

#include "PLWAHSet.h"
#include "utilities.h"

namespace PLWAHSpace{

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

	PLWAHSet codis;

	codis.fromVector(data);
	printf("original: \n");
	printBitmap(bitmap);
	printf("\n\n");
	printBitmap(codis.bitmap);

	vector<Int> result;
	codis.toVector(result);
	if (compareIntList(data, result)) {
		printf("check over\n");
	}
	else {
		printf("check failed\n");
	}
}

};

#endif
