#ifndef __TOTAL_TEST__
#define __TOTAL_TEST__
#include "realTest.hpp"

#include "WAHSet.h"
#include "PLWAHSet.h"
#include "COMPAXSet.h"
#include "CODISWAHSet.h"

void testAllAlgorithm() {
	RealData::RecordReader reader;
	reader.readData();
	int failCount = 0;
	failCount += totalFromToVector<WAHSpace::WAHSet>(reader);
	failCount += totalFromToVector<PLWAHSpace::PLWAHSet>(reader);
	failCount += totalFromToVector<COMPAXSpace::COMPAXSet>(reader);
	failCount += totalFromToVector<CODISWAHSpace::CODISWAHSet>(reader);
	failCount += randomIntersectionTest<WAHSpace::WAHSet>(reader, 100);
	failCount += randomIntersectionTest<PLWAHSpace::PLWAHSet>(reader, 100);
	failCount += randomIntersectionTest<COMPAXSpace::COMPAXSet>(reader, 100);
	failCount += randomIntersectionTest<CODISWAHSpace::CODISWAHSet>(reader, 100);

	printf("total failed %d\n", failCount);
}




#endif