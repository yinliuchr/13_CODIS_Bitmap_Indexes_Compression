#ifndef __INTER_BITMAP_COMPAX_TEST__
#define __INTER_BITMAP_COMPAX_TEST__

#include <algorithm>

#include "utilities.h"
#include "COMPAXSet.h"
namespace COMPAXSpace{

void setBitmapOne(Bitmap& bitmap) {
	bitmap.push_back(0x40000001);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x80700101);
	bitmap.push_back(0x00000006);
	bitmap.push_back(0x80070080);
	bitmap.push_back(0x40000001);
}

void setBitmapTwo(Bitmap& bitmap) {
	bitmap.push_back(0x80000701);
	bitmap.push_back(0x40000006);
	bitmap.push_back(0x40000001);
	bitmap.push_back(0x80000001);
	bitmap.push_back(0x80070000);
	bitmap.push_back(0x40000001);
}

void setBitmapThree(Bitmap& bitmap) {
	bitmap.push_back(0x80000701);
	bitmap.push_back(0x00000001);
	bitmap.push_back(0x80700101);
	bitmap.push_back(0x00000006);
	bitmap.push_back(0x80070000);
	bitmap.push_back(0x40000001);
}

void testSetIntersection() {
	Bitmap a, b, c;
	setBitmapOne(a);
	setBitmapTwo(b);
	setBitmapThree(c);

	vector<Int> A,B,C;
	dataGenFromWAH(A,a);
	dataGenFromWAH(B,b);
	dataGenFromWAH(C,c);

	COMPAXSet sa, sb, sc;
	sa.fromVector(A);
	sb.fromVector(B);
	COMPAXSet::setIntersection(sc, sa, sb);

	sc.toVector(A);

	printIntList(A);
	printIntList(C);
	if (compareIntList(A,C)) {
		printf("check over\n");
	}
	else {
		printf("check fail\n");
	}
	printf("\n\n");
}

void testRandom() {
	vector<Int> A,B, C;
	dataGen(A, 1000, 30);
	dataGen(B, 1500, 25);

	COMPAXSet a, b, c;

	a.fromVector(A);
	b.fromVector(B);

	C.resize(A.size() > B.size()? A.size():B.size());
	auto itr = std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), C.begin());
	C.resize(itr - C.begin());

	COMPAXSet::setIntersection(c, a, b);
	c.toVector(A);

	printIntList(A, 20);
	printf("\n");
	printIntList(C, 20);
	printf("\n");
	if (compareIntList(A, C)) {
		printf("check over\n");
	}
	else {
		printf("check fail\n");
	}
	printf("\n");

}

};

#endif