#ifndef __CODISWAH_SET__
#define __CODISWAH_SET__

#include <vector>
#include <cstdint>
#include <algorithm>
#include <intrin.h>
#include <array>

using std::vector;

#include "utilities.h"

namespace CODISWAHSpace{

class CODISWAHSet{
public:
	Bitmap bitmap; // a vector of words
	Int length;		// number of '1'
	Int max;	// position of last '1'

	inline CODISWAHSet(): length(0), max(0) {}
	inline ~CODISWAHSet(){;}

	void fromVector(const vector<Int>& v); //v 是从小到大排序的整数列表，从v生成位图
	void toVector(vector<Int>& v);
	void addLiteral(Word w);
	static void setIntersection(CODISWAHSet& result, const CODISWAHSet& set1, const CODISWAHSet& set2); // between two bitmap
	static void setUnion(CODISWAHSet& result, const CODISWAHSet& set1, const CODISWAHSet& set2); // between two bitmap
	
	inline double getBpi() const{return bitmap.size()/(length+0.0);}
	void clear(){bitmap.clear(); length = 0; max = 0;}

};

};
#endif