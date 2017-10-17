#include "COMPAXSet.h"
#include "utilities.h"

namespace COMPAXSpace{

class CodeWordDispatcher{
public:
	Word word[3];
	int counter[3];
	int pointer;

	void setWord(Word w) {
		if (_bittest((long*)&w, 31)) {
			pointer = 2;
			word[2] = w;
			counter[2] = 1;
		}
		else {
			switch(_bextr_u32(w, 29, 2)) {
			case 0: // 0fill
				word[2] = 0x80000000;
				counter[2] = w;
				pointer = 2;
				break;
			case 1: // LFL
				pointer = 0;
				word[0] = 0x80000000 | (_bextr_u32(w, 16, 8) << (_bextr_u32(w, 27, 2) * 8));
				word[1] = 0x80000000;
				word[2] = 0x80000000 | (_bextr_u32(w, 0, 8) << (_bextr_u32(w, 25, 2) * 8));
				counter[0] = 1;
				counter[1] = _bextr_u32(w, 8, 8);
				counter[2] = 1;
				break;
			case 2: // FLF
				pointer = 0;
				word[0] = 0x80000000;
				word[1] = 0x80000000 | (_bextr_u32(w, 8, 8) << (_bextr_u32(w, 27, 2) * 8));
				word[2] = 0x80000000;
				counter[0] = _bextr_u32(w, 16, 8);
				counter[1] = 1;
				counter[2] = _bextr_u32(w, 0, 8);
				break;
			default:
				printf("Meet a unknown code word when decoding\n");
				exit(1);
			}
		}
	}
	
	Word next(){
		Word ret = word[pointer];
		counter[pointer]--;
		if (counter[pointer] == 0) {
			pointer ++;
		}
		return ret;
	}
	
	bool hasNext() {
		return pointer != 3;
	}
};

bool checkContinue(CodeWordDispatcher& p, int& itr, const COMPAXSet& set){
	if(!p.hasNext()) {
		if (itr == set.bitmap.size()) {
			return false;
		}
		else {
			p.setWord(set.bitmap[itr]);
			itr ++;
		}
	}
	return true;
}

void COMPAXSet::setIntersection(COMPAXSet& result, const COMPAXSet& set1, const COMPAXSet& set2) {
	result.clear();
	if ((set1.length == 0) || (set2.length == 0)) {
		return;
	}

	CodeWordDispatcher p1, p2;
	int itr1 = 1;
	int itr2 = 1;
	p1.setWord(set1.bitmap[0]);
	p2.setWord(set2.bitmap[0]);

	while(checkContinue(p1, itr1, set1) && checkContinue(p2, itr2, set2)) {
		Word out = p1.next() & p2.next();
		result.addLiteral(out); 
	}
}

void COMPAXSet::setUnion(COMPAXSet& result, const COMPAXSet& set1, const COMPAXSet& set2) {
	result.clear();
	if ((set1.length == 0) || (set2.length == 0)) {
		return;
	}

	CodeWordDispatcher p1, p2;
	int itr1 = 1;
	int itr2 = 1;
	p1.setWord(set1.bitmap[0]);
	p2.setWord(set2.bitmap[0]);

	while(checkContinue(p1, itr1, set1) && checkContinue(p2, itr2, set2)) {
		Word out = p1.next() | p2.next();
		result.addLiteral(out); 
	}
}

};