#ifndef __TOTAL_EXP__
#define __TOTAL_EXP__

#include "compress.hpp"
#include "interBitmap.hpp"

#include "WAHSet.h"
#include "PLWAHSet.h"
#include "COMPAXSet.h"
#include "CODISWAHSet.h"

void writeInterStatSet(Experiment::InterStatSet is, const char* filename) {
	FILE* p = fopen(filename, "w");
	for (int i = 0; i < is.size(); i++) {
		is[i].writeFile(p);
	}
	fclose(p);
}

void totalExp() {
	RealData::RecordReader reader;
	reader.readData();

	Experiment::CompressionStat wahc, plwahc, compaxc, codisc;
	
	wahc = Experiment::compressionAll<WAHSpace::WAHSet>(reader);
	plwahc = Experiment::compressionAll<PLWAHSpace::PLWAHSet>(reader);
	compaxc = Experiment::compressionAll<COMPAXSpace::COMPAXSet>(reader);
	codisc = Experiment::compressionAll<CODISWAHSpace::CODISWAHSet>(reader);

	FILE* p = fopen("./compression.txt", "w");
	wahc.writeFile(p); plwahc.writeFile(p); compaxc.writeFile(p); codisc.writeFile(p);
	fclose(p);

	/*Experiment::InterStatSet wahi, wahu, plwahi, plwahu, compaxi, compaxu, codisi, codisu;
	for(int i = 0; i < 10000; i++) {
		int byte1 = rand()%8;
		int num1 = rand()%256;
		int byte2 = rand()%8;
		int num2 = rand()%256;

		wahi.push_back(Experiment::intersection<WAHSpace::WAHSet>(reader, byte1, num1, byte2, num2));
		plwahi.push_back(Experiment::intersection<PLWAHSpace::PLWAHSet>(reader, byte1, num1, byte2, num2));
		compaxi.push_back(Experiment::intersection<COMPAXSpace::COMPAXSet>(reader,byte1, num1, byte2, num2));
		codisi.push_back(Experiment::intersection<CODISWAHSpace::CODISWAHSet>(reader, byte1, num1, byte2, num2));

		wahu.push_back(Experiment::unions<WAHSpace::WAHSet>(reader, byte1, num1, byte2, num2));
		plwahu.push_back(Experiment::unions<PLWAHSpace::PLWAHSet>(reader, byte1, num1, byte2, num2));
		compaxu.push_back(Experiment::unions<COMPAXSpace::COMPAXSet>(reader,byte1, num1, byte2, num2));
		codisu.push_back(Experiment::unions<CODISWAHSpace::CODISWAHSet>(reader, byte1, num1, byte2, num2));
	}

	writeInterStatSet(wahi, "./wahIntersection.txt");
	writeInterStatSet(plwahi, "./plwahIntersection.txt");
	writeInterStatSet(compaxi, "./compaxIntersection.txt");
	writeInterStatSet(codisi, "./codisIntersection.txt");
	writeInterStatSet(wahu, "./wahUnion.txt");
	writeInterStatSet(plwahu, "./plwahUnion.txt");
	writeInterStatSet(compaxu, "./compaxUnion.txt");
	writeInterStatSet(codisu, "./codisUnion.txt");*/
}




#endif