#include <array>

#include "VectorChunker.h"
#include "CODISWAHSet.h"

namespace CODISWAHSpace{

Word combinedWord(Word literal, WordDescriptor::WordType filltype) {
	_bittestandreset((long*)&literal, 31);
	Word lz = _lzcnt_u32(literal);
	Word tz = _tzcnt_u32(literal);
	tz = tz/2;
	
	Word length = 32 - lz - tz*2;
	length = (length + 2)/3;

	if(length > 7) {
		return 0;
	}
	else {
		literal = literal >> tz*2;
		literal = literal << (23 - length*3);
		literal |= (length << 27);
		literal |= (tz << 23);
	}

	if (filltype == WordDescriptor::ALL_ONE) {
		literal = literal | 0x40000000;
	}
	literal = literal + 1;

	return literal;
}

bool isCarryFull(Word w) {
	int length = _bextr_u32(w, 27, 3);
	if(length == 0) {
		length = 27;
	}
	else{
		length = 23 - 3*length;
	}
	Word mask = 1;
	mask = (mask << length) - 1;

	if ((w&mask) == mask){
		return true;
	}
	return false;
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

void CODISWAHSet::fromVector(const vector<Int>& v)
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