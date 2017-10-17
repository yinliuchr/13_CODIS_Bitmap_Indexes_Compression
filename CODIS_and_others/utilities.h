#ifndef __UTILITIES__
#define __UTILITIES__

#include <stdint.h>
#include <vector>

using std::vector;

typedef uint32_t Word;
typedef int64_t Int;

typedef vector<Word> Bitmap;

struct WordDescriptor{
	enum WordType {LITERAL=0, ALL_ZERO, ALL_ONE, END};
	Word word;
	WordType type;

	WordDescriptor(): word(0), type(WordType::LITERAL) {}
};

void dataGen(vector<Int>& v, int number, int step);
void dataGenFromWAH(vector<Int>& v, Bitmap& bitmap);

void printIntList(vector<Int>& v, int num = 1000);
void printBitmap(Bitmap& bitmap);
bool compareIntList(vector<Int>& v1, vector<Int>& v2);

namespace RealData{
	struct Readable{
		unsigned char srcIp[4];
		unsigned char destIp[4];
	};

	union Record{
		Readable readable;
		unsigned char bytes[8];
		void print() {
			printf("%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu\n", 
				readable.srcIp[0], readable.srcIp[1], readable.srcIp[2], readable.srcIp[3],
				readable.destIp[0], readable.destIp[1], readable.destIp[2], readable.destIp[3]);
		}
		const Record& operator=(const Record& r) {
			memcpy(bytes, r.bytes, 8);
		}
	};

	class RecordReader{
	public:
		vector<Record> data;
		void readData();
		void extractIntegers(vector<Int>& v, int bytes, int value);
	};

	
};
#endif