#include "WAHSet.h"

namespace WAHSpace{

void decodeLiteral(Word wd, vector<Int>& v, Int prog) {
	while(wd != 0) {
		Int tz = _tzcnt_u32(wd);
		v.push_back(tz + prog);
		_bittestandreset((long*)&wd, tz);
	}
}

void addOnes(vector<Int>& v, Int start, Int end) {
	for (Int i = start; i < end; i++) {
		v.push_back(i);
	}
}


void decodeWord(Word wd, vector<Int>& v, Int& prog) {
	if (_bittestandreset((long*)&wd, 31)) {
		decodeLiteral(wd, v, prog);
		prog += 31;
	}
	else if(_bittestandreset((long*)&wd, 30)){
		addOnes(v, prog, prog + 31*wd);
		prog += 31*wd;
	}
	else {
		prog += 31*wd;
	}
}

void WAHSet::toVector(vector<Int>& v) {
	v.clear();
	Int prog = 0;
	for(int i= 0; i < bitmap.size(); i++) {
		decodeWord(bitmap[i], v, prog);
	}
}
};