#include <array>

#include "VectorChunker.h"
#include "PLWAHSet.h"

namespace PLWAHSpace{

Word combinedWord(Word literal, WordDescriptor::WordType filltype) {
	if (__popcnt(literal) != 2) {
		return 0;
	}
	Word lz = 31 - _tzcnt_u32(literal);
	literal = lz << 25;
	if (filltype == WordDescriptor::ALL_ONE) {
		_bittestandset((long*)&literal, 30);
	}
	literal += 1;
	return literal;
}

bool isCarryFull(Word w) {
	return _bextr_u32(w, 0, 25) == 0x01FFFFFF;
}

bool tryCombine(Bitmap& bitmap, WordDescriptor wd) {
	Word lastword = bitmap.back();
	Word carried = combinedWord(lastword, wd.type);
	if (carried == 0) {
		return false;
	}
	else {
		bitmap.pop_back();
		bitmap.push_back(carried);
		return true;
	}
}

void addWord(Bitmap& bitmap, WordDescriptor wd, WordDescriptor::WordType lastType) {
	bool absorbed = false;

	if (wd.type == WordDescriptor::LITERAL) { // new word
			bitmap.push_back(wd.word);
			absorbed = true;
	}
	else if (lastType == WordDescriptor::LITERAL) { // literal + fill
		absorbed = tryCombine(bitmap, wd);
	}
	else if (lastType == wd.type) { // same fill
		if (isCarryFull(*(bitmap.end()-1))) {
			absorbed = false;
		}
		else{
			(*(bitmap.end() - 1))++;
			absorbed = true;
		}
	}

	if(absorbed) {
		return;
	}

	if (wd.type == WordDescriptor::ALL_ZERO) { // different fill
		bitmap.push_back(0x00000001);
	}
	else { // different fill
		bitmap.push_back(0x40000001);
	}
}

void PLWAHSet::fromVector(const vector<Int>& v)
{
	if (v.size() == 0) {
		bitmap.clear();
		return;
	}
	length = v.size();
	Int chunkMax = (v.back()/31) + 1;

	bitmap.clear();
	bitmap.reserve(chunkMax/4); // just estimation

	VectorChunker vc(v);
	WordDescriptor wd;
	WordDescriptor::WordType lastType = WordDescriptor::END;

	wd = vc.next();
	while(wd.type != WordDescriptor::END) {
		addWord(bitmap, wd, lastType);
		lastType = wd.type;
		wd = vc.next();
	}
	this->length = v.size();
	this->max = v.back();
}

};