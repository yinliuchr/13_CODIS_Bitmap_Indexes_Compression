#ifndef __COMPAXSET__
#define __COMPAXSET__

#include <vector>
#include <cstdint>
#include <algorithm>
#include <intrin.h>
#include <array>

using std::vector;

#include "utilities.h"

namespace COMPAXSpace{

class COMPAXSet{
public:
	Bitmap bitmap; // a vector of words
	Int length;		// number of '1'
	Int max;	// position of last '1'

	inline COMPAXSet(): length(0), max(0) {}
	inline ~COMPAXSet(){;}

	void fromVector(const vector<Int>& v); //v 是从小到大排序的整数列表，从v生成位图
	void toVector(vector<Int>& v);
	void addLiteral(Word w);
	static void setIntersection(COMPAXSet& result, const COMPAXSet& set1, const COMPAXSet& set2); // between two bitmap
	static void setUnion(COMPAXSet& result, const COMPAXSet& set1, const COMPAXSet& set2); // between two bitmap
	
	inline double getBpi() const{return bitmap.size()/(length+0.0);}
	void clear(){bitmap.clear(); length = 0; max = 0;}

};

};
#endif