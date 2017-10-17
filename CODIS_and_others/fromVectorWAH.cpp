#include <array>

#include "VectorChunker.h"
#include "WAHSet.h"

namespace WAHSpace{

bool isWordFull(Word w) {
	Word counter = _bextr_u32(w, 0, 30);
	return counter == 0x3FFFFFFF;
}


void addWord(Bitmap& bitmap, WordDescriptor wd, WordDescriptor::WordType lastType) {
	bool absorbed = false;

	if(wd.type == WordDescriptor::LITERAL) {
		bitmap.push_back(wd.word);
		absorbed = true;
	}
	else if (lastType == wd.type) {
		if (isWordFull(*(bitmap.end() - 1))) {
			absorbed = false;
		}
		else {
			(*(bitmap.end() - 1)) ++;
			absorbed = true;
		}
	}

	if(absorbed){
		return;
	}

	if(wd.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0x40000001);
	}
	else {
		bitmap.push_back(0x00000001);
	}
}

void WAHSet::fromVector(const vector<Int>& v)
{
	if (v.size() == 0) {
		bitmap.clear();
		return;
	}

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
