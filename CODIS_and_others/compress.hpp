#ifndef __COMPRESS__
#define __COMPRESS__
#include <chrono> 
using namespace std::chrono;

#include "utilities.h"

namespace Experiment{
struct CompressionStat{
	Int byte;
	double encodingSecond;
	double decodingSecond;

	const CompressionStat& operator=(const CompressionStat& b) {
		byte = b.byte;
		encodingSecond = b.encodingSecond;
		decodingSecond = b.decodingSecond;
		return b;
	}

	CompressionStat() {
		byte = 0;
		encodingSecond = 0.0f;
		decodingSecond = 0.0f;
	}

	void add(const CompressionStat& b) {
		byte += b.byte;
		encodingSecond += b.encodingSecond;
		decodingSecond += b.decodingSecond;
	}

	void print() {
		printf("%ld,%lf,%lf\n", byte, encodingSecond, decodingSecond);
	}

	void writeFile(FILE* p) {
		fprintf_s(p, "%ld,%lf,%lf\n", byte, encodingSecond, decodingSecond);
	}
};

template <class T>
CompressionStat compressionOneList(RealData::RecordReader& reader, int byte, int num) {
	vector<Int> data, result;
	reader.extractIntegers(data, byte, num);
	T encoder;

	auto time_start = system_clock::now();
	encoder.fromVector(data);
	auto time_end = system_clock::now();
	double time_encoding = (duration_cast<microseconds>(time_end - time_start).count());
	time_start = system_clock::now();
	encoder.toVector(result);
	time_end = system_clock::now();
	double time_decoding = (duration_cast<microseconds>(time_end - time_start).count());

	CompressionStat stat;
	stat.byte = encoder.bitmap.size();
	stat.encodingSecond = time_encoding;
	stat.decodingSecond = time_decoding;

	return stat;
}

template<class T>
CompressionStat compressionAll(RealData::RecordReader& reader) {
	CompressionStat stat;
	stat.byte = 0;
	stat.encodingSecond = 0;
	stat.decodingSecond = 0;

	for(int byte = 0; byte < 8; byte ++) {
		for (int num = 0; num < 256; num ++) {
			auto A = compressionOneList<T>(reader, byte, num);
			stat.add(A);
			//stat.print();
		}
	}
	
	return stat;
}


};

#endif