#include "PLWAHSet.h"

namespace PLWAHSpace{

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
	else {
		int lz = _bextr_u32(wd, 25, 5);
		if (lz != 0){
			Word literal = 0x80000000 >> lz;
			decodeLiteral(literal, v, prog);
			prog += 31;
		}

		Word counter = _bextr_u32(wd, 0, 25);
		if (_bittest((long*)&wd, 30)) {
			addOnes(v, prog, prog + 31*counter);
		}
		prog += 31*counter;
	}
}

void PLWAHSet::toVector(vector<Int>& v) {
	v.clear();
	Int prog = 0;
	for(int i= 0; i < bitmap.size(); i++) {
		Word curr = bitmap[i];
		decodeWord(curr, v, prog);
	}
}
};