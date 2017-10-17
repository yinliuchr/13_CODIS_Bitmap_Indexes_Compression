#include "COMPAXSet.h"

namespace COMPAXSpace{

void decodeLiteral(Word wd, vector<Int>& v, Int prog) {
	_bittestandreset((long*)&wd, 31);
	while(wd != 0) {
		Int tz = _tzcnt_u32(wd);
		v.push_back(tz + prog);
		_bittestandreset((long*)&wd, tz);
	}
}

void decodeWord(Word wd, vector<Int>& v, Int& prog) {
	if (_bittestandreset((long*)&wd, 31)) {
		decodeLiteral(wd, v, prog);
		prog += 31;
	}
	else{
		Word flag = _bextr_u32(wd, 29, 2);
		if (flag == 0) {
			Word counter = _bextr_u32(wd, 0, 29);
			prog += 31*counter;
		}
		else if (flag == 1) { //LFL
			Word LOffset = _bextr_u32(wd, 27, 2);
			Word literal = _bextr_u32(wd, 16, 8) << (LOffset * 8);
			decodeLiteral(literal, v, prog);
			prog += 31;
			
			prog += _bextr_u32(wd, 8, 8) * 31;

			LOffset = _bextr_u32(wd, 25, 2);
			literal = _bextr_u32(wd, 0, 8) << (LOffset * 8);
			decodeLiteral(literal, v, prog);
			prog += 31;
		}
		else { // FLF
			prog += _bextr_u32(wd, 16, 8) * 31;
			Word LOffset = _bextr_u32(wd, 27 ,2);
			Word literal = _bextr_u32(wd, 8, 8) << (LOffset * 8);
			decodeLiteral(literal, v, prog);
			prog += 31;
			prog += _bextr_u32(wd, 0, 8) * 31;
		}
	}
	
}

void COMPAXSet::toVector(vector<Int>& v) {
	v.clear();
	Int prog = 0;
	for(int i= 0; i < bitmap.size(); i++) {
		Word curr = bitmap[i];
		decodeWord(curr, v, prog);
	}
}

};