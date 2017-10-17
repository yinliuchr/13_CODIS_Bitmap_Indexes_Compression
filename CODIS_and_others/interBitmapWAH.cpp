#include "WAHSet.h"
#include "utilities.h"

namespace WAHSpace{

class CodeWordDispatcher{
public:
	Word word;
	int counter;

	void setWord(Word w) {
		if (_bittest((long*)&w, 31)) {
			word = w;
			counter = 1;
		}
		else if (_bittestandreset((long*)&w, 30)){
			word = 0xFFFFFFFF;
			counter = w;
		}
		else {
			word = 0x80000000;
			counter = w;
		}
	}
	
	Word next(){
		counter--;
		return word;
	}
	
	bool hasNext() {
		return counter != 0;
	}
};

bool checkContinue(CodeWordDispatcher& p, int& itr, const WAHSet& set){
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

void WAHSet::setIntersection(WAHSet& result, const WAHSet& set1, const WAHSet& set2) {
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

void WAHSet::setUnion(WAHSet& result, const WAHSet& set1, const WAHSet& set2) {
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