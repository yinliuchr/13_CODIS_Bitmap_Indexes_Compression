#ifndef __REAL_DATA_TEST__
#define __REAL_DATA_TEST__

#include "utilities.h"

void testRecordStructure(){
	RealData::Record r;
	r.readable.srcIp[0] = 1;
	r.readable.srcIp[1] = 2;
	r.readable.srcIp[2] = 3;
	r.readable.srcIp[3] = 4;
	
	r.readable.destIp[0] = 5;
	r.readable.destIp[1] = 6;
	r.readable.destIp[2] = 7;
	r.readable.destIp[3] = 8;


	for(int i = 0; i < 12; i ++) {
		printf("%hhu\t", r.bytes[i]);
	}
}

void readTest() {
	RealData::RecordReader reader;
	reader.readData();
	printf("data length: %d\n", reader.data.size());
	//for(int i = 0; i < data.size(); i ++) {
	//	data[i].print();
	//	system("pause");
	//}
}

#endif