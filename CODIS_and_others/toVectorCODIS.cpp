#include "CODISWAHSet.h"

namespace CODISWAHSpace{

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
		Word length = _bextr_u32(wd, 27, 3);
		if (length == 0) {
			Word counter = _bextr_u32(wd, 0, 27);
			if (_bittest((long*)&wd, 30)) { // one fill
				addOnes(v, prog, prog + counter * 31);
			}
			prog += counter * 31;
		}
		else {
			Word literal = _bextr_u32(wd, 23 - length*3, length*3);
			Int position = _bextr_u32(wd, 23, 4) * 2;
			literal = literal << position;
			decodeLiteral(literal, v, prog);
			prog += 31;

			Word counter = _bextr_u32(wd, 0, 23 - length*3);
			if (_bittest((long*)&wd, 30)) {
				addOnes(v, prog, prog + counter * 31);
			}
			prog += counter * 31;
		}
	}
}

void CODISWAHSet::toVector(vector<Int>& v) {
	v.clear();
	Int prog = 0;
	for(int i= 0; i < bitmap.size(); i++) {
		Word curr = bitmap[i];
		decodeWord(curr, v, prog);
	}
}

};