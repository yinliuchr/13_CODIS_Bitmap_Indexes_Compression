#ifndef __ALGORITHM_TEST__
#define __ALGORITHM_TEST__

#include "utilities.h"

Word presets[] = {0x40000001, 0x00000001, 0x80700101, 0x00000001, 0x80070080, 0x40000001,0x40000001, 0x00000001, 0x80700101, 0x00000006, 0x80070080, 0x40000001,0x80000701, 0x40000006, 0x40000001, 0x80000001, 0x80070000, 0x40000001};

void setBitmap(Bitmap& bitmap, int index) {
	Word* data = presets+index*6;
	for(int i=0; i< 6; i++) {
		bitmap.push_back(data[i]);
	}
}

template <class T>
void testFromToVector(){
	vector<Int> data;
	Bitmap bitmap;
	setBitmap(bitmap, 0);
	dataGenFromWAH(data, bitmap);

	T codis;

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

template <class T>
void testSetIntersection() {
	Bitmap a, b, c;
	setBitmap(a, 0);
	setBitmap(b, 1);
	
	vector<Int> A,B,C;
	dataGenFromWAH(A,a);
	dataGenFromWAH(B,b);

	C.resize(A.size() > B.size()? A.size(): B.size());
	auto itr = std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), C.begin());
	C.resize(itr - C.begin());

	T sa, sb, sc;
	sa.fromVector(A);
	sb.fromVector(B);
	T::setIntersection(sc, sa, sb);

	sc.toVector(A);

	printIntList(A);
	printIntList(C);
	if (compareIntList(A,C)) {
		printf("check over\n");
	}
	else {
		printf("check fail\n");
	}
}

template <class T>
void testRandom() {
	vector<Int> A,B, C;
	dataGen(A, 1000, 30);
	dataGen(B, 1500, 25);

	T a, b, c;

	a.fromVector(A);
	b.fromVector(B);

	C.resize(A.size() > B.size()? A.size():B.size());
	auto itr = std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), C.begin());
	C.resize(itr - C.begin());

	T::setIntersection(c, a, b);
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

template <class T>
void totalTest() {
	testFromToVector<T>();
	testSetIntersection<T>();
	testRandom<T>();
}
#endif