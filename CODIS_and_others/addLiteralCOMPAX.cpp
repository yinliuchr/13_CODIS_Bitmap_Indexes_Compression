#include "COMPAXSet.h"

namespace COMPAXSpace{

void COMPAXSet::addLiteral(Word w) {
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

	if (_bittest((long*)&last, 31)) {
		bitmap.push_back(w);
	}
	else if (last == 0x1FFFFFFF) {
		bitmap.push_back(w);
	}
	else if (_bittest((long*)&w, 31)){
		bitmap.push_back(w);
	}
	else {
		(*(bitmap.end() - 1)) ++;
	}
}
};