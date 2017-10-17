#include "utilities.h"

void printIntList(vector<Int>& v, int num) {
	if (num > v.size()) num = v.size();
	for(int i = 0; i < v.size(); i ++){
		printf("%lld\t", v[i]);
	}
	printf("\n");
}

void printBitmap(Bitmap& bitmap) {
	for(int i = 0; i < bitmap.size(); i++) {
		printf("0x%08lx\t", bitmap[i]);
	}
	printf("\n");
}


bool compareIntList(vector<Int>& v1, vector<Int>& v2) {
	if (v1.size() != v2.size()) {
		return false;
	}

	for (int i=0; i<v1.size(); i++) {
		if (v1[i] != v2[i]) {
			return false;
		}
	}

	return true;
}

void dataGen(vector<Int>& v, int number, int step)
{
	v.clear();
	v.reserve(number);

	srand(131);

	int x = 0;
	for(int i = 0;i<number;i++){
		x+= (rand()%step + 1);
		v.push_back(x);
	}
}

void dataGenFromWAH(vector<Int>& v, Bitmap& bitmap) {
	v.clear();
	
	Int prog = 0;
	for(unsigned int i = 0; i < bitmap.size(); i++) {
		Word current = bitmap[i];
		if (_bittestandreset((long*)&current, 31)) {
			while(current != 0) {
				int tz = _tzcnt_u32(current);
				v.push_back(tz + prog);
				_bittestandreset((long*)&current, tz);
			}
			prog += 31;
		}
		else if (_bittestandreset((long*)&current, 30)){
			Int range = current * 31;
			for(Int i = 0; i < range; i++) {
				v.push_back(prog + i);
			}
			prog += range;
		}
		else {
			Int range = current * 31;
			prog += range;
		}
	}
}

void RealData::RecordReader::readData() {
	data.clear();
	FILE* p = fopen("./traffic.txt", "r");
	RealData::Record r;
	int srcIP0, srcIP1, srcIP2, srcIP3, destIP0, destIP1, destIP2, destIP3;
	Int num = 0;
	while(!feof(p)) {
		int count = fscanf_s(p, "%d,%d,%d,%d,%*d,%d,%d,%d,%d,%*d,%*d\n", 
			&srcIP0, &srcIP1, &srcIP2, &srcIP3, &destIP0, &destIP1, &destIP2, &destIP3); 
		r.readable.srcIp[0] = srcIP0;
		r.readable.srcIp[1] = srcIP1;
		r.readable.srcIp[2] = srcIP2;
		r.readable.srcIp[3] = srcIP3;
		r.readable.destIp[0] = destIP0;
		r.readable.destIp[1] = destIP1;
		r.readable.destIp[2] = destIP2;
		r.readable.destIp[3] = destIP3;
		
		if (count != 8)  {
			printf("read %d quit.\n", count);
			break;
		}
		if (data.size() % 100000 == 0) {
			printf("read %d record\n", data.size());
		}
		data.push_back(r);
		
		num ++;
		if (num == 10000) break;
	}

	fclose(p);
}

void RealData::RecordReader::extractIntegers(vector<Int>& v, int bytes, int value) {
	v.clear();
	for(Int i=0; i<data.size(); i++) {
		if(data[i].bytes[bytes] == value) {
			v.push_back(i);
		}
	}
}