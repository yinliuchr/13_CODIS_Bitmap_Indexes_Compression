#ifndef __ENCODING_CODIS_TEST__
#define __ENCODING_CODIS_TEST__

#include "CODISWAHSet.h"
#include "utilities.h"

namespace CODISWAHSpace{

void setBitmap(Bitmap& bitmap) {
	bitmap.push_back(0x40000001);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x80700101);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x80070080);
	bitmap.push_back(0x40000001);
}

void testFromToVector(){
	vector<Int> data;
	Bitmap bitmap;
	setBitmap(bitmap);
	dataGenFromWAH(data, bitmap);

	CODISWAHSet codis;

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
