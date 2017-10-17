#include "PLWAHSet.h"
#include "utilities.h"

namespace PLWAHSpace{

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
			pointer = 1;
			Word literal = _bextr_u32(w, 25, 5);
			if(literal != 0) {
				pointer = 0;
				literal = 0x80000000 >> literal;
				word[0] = 0x80000000 | literal;
			}
			counter = _bextr_u32(w, 0 ,25);
			
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

bool checkContinue(CodeWordDispatcher& p, int& itr, const PLWAHSet& set){
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

void PLWAHSet::setIntersection(PLWAHSet& result, const PLWAHSet& set1, const PLWAHSet& set2) {
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

void PLWAHSet::setUnion(PLWAHSet& result, const PLWAHSet& set1, const PLWAHSet& set2) {
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