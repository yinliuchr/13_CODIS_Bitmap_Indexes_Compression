#include "PLWAHSet.h"

namespace PLWAHSpace{

void PLWAHSet::addLiteral(Word w) {
	Word last; 
	if (bitmap.size() == 0) {
		last = 0x80000001;
	}
	else {
		last= bitmap.back();
	}

	if (w == 0x80000000) {
		w = 0x00000001;
	}
	else if (w == 0xFFFFFFFF) {
		w = 0x40000001;
	}

	switch(_bextr_u32(last, 30, 2)) {
	case 0: // zero fill
		if(w == 0x00000001 && last != 0x01FFFFFF) {
			(*(bitmap.end() - 1)) ++;
		}
		else {
			bitmap.push_back(w);
		}
		break;
	case 1: // one fill
		if (w == 0x40000001 && last != 0x41FFFFFF) {
			 (*(bitmap.end() - 1)) ++;
		}
		else {
			bitmap.push_back(w);
		}
		break;
	case 2:
	case 3:
		bitmap.push_back(w);
		break;
	}
}
};