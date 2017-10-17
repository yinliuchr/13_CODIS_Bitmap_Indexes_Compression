#ifndef __REAL_TEST__
#define __REAL_TEST__
#include <cstdlib>

#include "utilities.h"

template <class T>
int testRealFromToVector(RealData::RecordReader& reader, int byte=0, int num=12){
	vector<Int> datain, dataout;

	reader.extractIntegers(datain, byte, num);

	T encoder;
	encoder.fromVector(datain);
	encoder.toVector(dataout);

	if (compareIntList(datain, dataout)) {
		printf("check over\n");
		return 0;
	}
	else{
		printf("check failed\n");
		return 1;
	}
}

template <class T>
int testRealIntersection(RealData::RecordReader& reader, int byte1, int num1, int byte2, int num2){
	vector<Int> data1, data2, dataStd, dataOut;
	
	reader.extractIntegers(data1, byte1, num1);
	reader.extractIntegers(data2, byte2, num2);

	dataStd.resize(data1.size() > data2.size()?data1.size() : data2.size());
	auto endItr = std::set_intersection(data1.begin(), data1.end(), data2.begin(), data2.end(), dataStd.begin());
	dataStd.resize(endItr - dataStd.begin());

	T E1, E2, Eout;
	E1.fromVector(data1);
	E2.fromVector(data2);
	T::setIntersection(Eout, E1, E2);

	Eout.toVector(dataOut);

	if (compareIntList(dataOut, dataStd)) {
		printf("check over\n");
		return 0;
	}
	else {
		printf("check failed\n");
		return 1;
	}
}


template<class T>
int randomIntersectionTest(RealData::RecordReader& reader, int batchSize) {
	int failCount = 0;
	for(int i=0; i<batchSize; i++) {
		failCount += testRealIntersection<T>(reader, rand()%8, rand()%256, rand()%8, rand()%256);
	}
	return failCount;
}

template <class T>
int totalFromToVector(RealData::RecordReader& reader) {
	int failCount = 0;
	for(int bytes = 0; bytes < 8; bytes ++) {
		for(int num = 0; num < 256; num ++) {
			printf("start bytes: %d, value %d\n", bytes, num);
			failCount += testRealFromToVector<T>(reader, bytes, num);
		}
	}
	printf("total failed :%d\n", failCount);
	return failCount;
}



#endif