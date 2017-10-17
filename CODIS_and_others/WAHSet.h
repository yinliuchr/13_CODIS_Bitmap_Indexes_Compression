#ifndef __WAHSET__
#define __WAHSET__
#include <vector>
#include <cstdint>
#include <algorithm>
#include <intrin.h>
#include <array>

using std::vector;

#include "utilities.h"

namespace WAHSpace{

class WAHSet{
public:
	Bitmap bitmap; // a vector of words
	Int length;		// number of '1'
	Int max;	// position of last '1'

	inline WAHSet(): length(0), max(0) {}
	inline ~WAHSet(){;}

	void fromVector(const vector<Int>& v); //v 是从小到大排序的整数列表，从v生成位图
	void toVector(vector<Int>& v);
	void addLiteral(Word w);
	static void setIntersection(WAHSet& result, const WAHSet& set1, const WAHSet& set2); // between two bitmap
	static void setUnion(WAHSet& result, const WAHSet& set1, const WAHSet& set2); // between two bitmap
	
	inline double getBpi() const{return bitmap.size()/(length+0.0);}
	void clear(){bitmap.clear(); length = 0; max = 0;}

};

};
#endif