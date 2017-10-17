#include "CODISWAHSet.h"
#include "utilities.h"

namespace CODISWAHSpace{

class CodeWordDispatcher{
public:
	Word word[2];
	int counter;
	int pointer;

	void setWord(Word w) {
		if (_bittest((long*)&w, 31)) {
			pointer = 1;
			word[1] = w;
			counter = 1;
		}
		else {
			Word length = _bextr_u32(w, 27, 3);
			if (length == 0) {
				counter = _bextr_u32(w, 0 ,27);
				pointer = 1;
			}
			else {
				counter = _bextr_u32(w, 0, 23 - length*3);
				Word literal = _bextr_u32(w, 23 - length*3, length*3);
				Int position = _bextr_u32(w, 23, 4) * 2;
				literal = literal << position;
				word[0] = literal | 0x80000000;
				pointer = 0;
			}

			if (_bittest((long*)&w, 30)) {
				word[1] = 0xFFFFFFFF;
			}
			else {
				word[1] = 0x80000000;
			}
		}
	}
	
	Word next(){
		if (pointer == 0) {
			pointer = 1;
			return word[0];
		}
		else {
			counter--;
			return word[1];
		}
	}
	
	bool hasNext() {
		return counter != 0 || pointer != 1;
	}
};

bool checkContinue(CodeWordDispatcher& p, int& itr, const CODISWAHSet& set){
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

void CODISWAHSet::setIntersection(CODISWAHSet& result, const CODISWAHSet& set1, const CODISWAHSet& set2) {
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

void CODISWAHSet::setUnion(CODISWAHSet& result, const CODISWAHSet& set1, const CODISWAHSet& set2) {
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