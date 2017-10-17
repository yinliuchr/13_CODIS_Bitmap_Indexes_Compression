#ifndef __INTER_BITMAP__
#define __INTER_BITMAP__

#include <chrono> 
using namespace std::chrono;

#include "utilities.h"
namespace Experiment{
struct InterStat {
	int lengthA;
	int lengthB;
	int lengthOut;
	double second;

	void writeFile(FILE* p) {
		fprintf_s(p, "%d, %d, %d, %lf\n", lengthA, lengthB, lengthOut, second);
	}
};

typedef vector<InterStat> InterStatSet;

template <class T>
InterStat intersection(RealData::RecordReader& reader, int byte1, int num1, int byte2, int num2) {
	vector<Int> data1, data2, dataOut;
	
	reader.extractIntegers(data1, byte1, num1);
	reader.extractIntegers(data2, byte2, num2);

	T E1, E2, Eout;
	E1.fromVector(data1);
	E2.fromVector(data2);

	auto start = system_clock::now();
	T::setIntersection(Eout, E1, E2);
	auto end = system_clock::now();

	Eout.toVector(dataOut);
	InterStat stat;
	stat.lengthA = data1.size();
	stat.lengthB = data2.size();
	stat.lengthOut = dataOut.size();
	stat.second  = duration_cast<microseconds>(end - start).count();
	
	return stat;
}

template <class T>
InterStat unions(RealData::RecordReader& reader, int byte1, int num1, int byte2, int num2) {
	vector<Int> data1, data2, dataOut;
	
	reader.extractIntegers(data1, byte1, num1);
	reader.extractIntegers(data2, byte2, num2);

	T E1, E2, Eout;
	E1.fromVector(data1);
	E2.fromVector(data2);

	auto start = system_clock::now();
	T::setUnion(Eout, E1, E2);
	auto end = system_clock::now();

	Eout.toVector(dataOut);
	InterStat stat;
	stat.lengthA = data1.size();
	stat.lengthB = data2.size();
	stat.lengthOut = dataOut.size();
	stat.second  = duration_cast<microseconds>(end - start).count();
	
	return stat;
}



};



#endif