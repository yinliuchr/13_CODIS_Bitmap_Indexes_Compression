#include <array>

#include "VectorChunker.h"
#include "COMPAXSet.h"

namespace COMPAXSpace{

enum COMPAXStatus {NONE, LITERAL, FILL, LF, FL, FLF};

bool isFillFull(Word w) {
	return w == 0x1FFFFFFF;
}

bool isCombinableFill(Word w) {
	return w < 0x00000100;
}

int combinableLiteralOffset(Word w) {
	_bittestandreset((long*)&w, 31);
	if ((w & 0x000000FF) == w) {
		return 0;
	}
	else if ((w & 0x0000FF00) == w) {
		return 1;
	}
	else if ((w & 0x00FF0000) == w) {
		return 2;
	}
	else if ((w & 0xFF000000) == w) {
		return 3;
	}
	else {
		return 4;
	}
}

COMPAXStatus combineLFL(Bitmap& bitmap) {
	Bitmap::iterator itr = bitmap.end() - 3;
	
	Word L1, F, L2;
	L1 = *itr; itr ++;
	F = *itr; itr ++;
	L2 = *itr;


	if (!isCombinableFill(F)) {
		return LITERAL;
	}

	Word L2Offset = combinableLiteralOffset(L2);
	if (L2Offset == 4) {
		return NONE;
	}

	Word L1Offset = combinableLiteralOffset(L1);
	if (L1Offset == 4) {
		return FL;
	}

	F = F << 8;
	F |= _bextr_u32(L2, L2Offset*8, 8);
	F |= (_bextr_u32(L1, L1Offset*8, 8) << 16);
	F |= 0x20000000;
	F |= (L1Offset << 27);
	F |= (L2Offset << 25);

	bitmap.pop_back(); bitmap.pop_back(); bitmap.pop_back();
	bitmap.push_back(F);
	return NONE;
}

COMPAXStatus combineFLF(Bitmap& bitmap) {
	Bitmap::iterator itr = bitmap.end() - 3;
	
	Word F1, L, F2;
	F1 = *itr; itr ++;
	L = *itr; itr ++;
	F2 = *itr;

	if (!isCombinableFill(F2)) {
		return NONE;
	}

	Word LOffset = combinableLiteralOffset(L);
	if (LOffset == 4) {
		return FILL;
	}

	if (!isCombinableFill(F1)) {
		return LF;
	} 

	F2 |= _bextr_u32(L, LOffset*8, 8) << 8;
	F2 |= (F1 << 16);
	F2 |= (LOffset << 27);
	F2 |= 0x40000000;

	bitmap.pop_back(); bitmap.pop_back(); bitmap.pop_back();
	bitmap.push_back(F2);
	return FLF;
}

COMPAXStatus basedOnNone(Bitmap& bitmap, WordDescriptor& w) {
	if(w.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0xFFFFFFFF);
		return LITERAL;
	}
	else if (w.type == WordDescriptor::ALL_ZERO) {
		bitmap.push_back(0x00000001);
		return FILL;
	}
	else{
		bitmap.push_back(w.word);
		return LITERAL;
	}
}

COMPAXStatus basedOnLiteral(Bitmap& bitmap, WordDescriptor& w) {
	if(w.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0xFFFFFFFF);
		return LITERAL;
	}
	else if (w.type == WordDescriptor::ALL_ZERO) {
		bitmap.push_back(0x00000001);
		return LF;
	}
	else{
		bitmap.push_back(w.word);
		return LITERAL;
	}
}

COMPAXStatus basedOnFill(Bitmap& bitmap, WordDescriptor& w) {
	if(w.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0xFFFFFFFF);
		return LITERAL;
	}
	else if (w.type == WordDescriptor::ALL_ZERO) {
		Word last = bitmap.back();
		if(isFillFull(last)) {
			bitmap.push_back(0x00000001);
		}
		else {
			(*(bitmap.end() - 1)) ++;
		}
		return FILL;
	}
	else{
		bitmap.push_back(w.word);
		return FL;
	}
}

COMPAXStatus basedOnLF(Bitmap& bitmap, WordDescriptor& w) {
	if(w.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0xFFFFFFFF);
		return LITERAL;
	}
	else if (w.type == WordDescriptor::ALL_ZERO) {
		Word last = bitmap.back();
		if(isFillFull(last)) {
			bitmap.push_back(0x00000001);
		}
		else {
			(*(bitmap.end() - 1)) ++;
		}
		return LF;
	}
	else{
		bitmap.push_back(w.word);
		return combineLFL(bitmap);
	}
}

COMPAXStatus basedOnFL(Bitmap& bitmap, WordDescriptor& w) {
	if(w.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0xFFFFFFFF);
		return LITERAL;
	}
	else if (w.type == WordDescriptor::ALL_ZERO) {
		bitmap.push_back(0x00000001);
		return combineFLF(bitmap);
	}
	else{
		bitmap.push_back(w.word);
		return LITERAL;
	}
}

COMPAXStatus basedOnFLF(Bitmap& bitmap, WordDescriptor& w) {
	if(w.type == WordDescriptor::ALL_ONE) {
		bitmap.push_back(0xFFFFFFFF);
		return LITERAL;
	}
	else if (w.type == WordDescriptor::ALL_ZERO) {
		Word last = bitmap.back();
		if (_bextr_u32(last, 0, 8) == 0x000000FF) {
			bitmap.push_back(0x00000001);
			return FILL;
		}
		else {
			(*(bitmap.end() - 1)) ++;
			return FLF;
		}
	}
	else{
		bitmap.push_back(w.word);
		return LITERAL;
	}
}

void addWord(Bitmap& bitmap, WordDescriptor& w, COMPAXStatus& status) {
	switch (status)
	{
	case NONE:
		status = basedOnNone(bitmap, w);
		break;
	case LITERAL:
		status = basedOnLiteral(bitmap, w);
		break;
	case FILL:
		status = basedOnFill(bitmap, w);
		break;
	case LF:
		status = basedOnLF(bitmap, w);
		break;
	case FL:
		status = basedOnFL(bitmap, w);
		break;
	case FLF:
		status = basedOnFLF(bitmap, w);
		break;
	}
}

void COMPAXSet::fromVector(const vector<Int>& v)
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
	COMPAXStatus status = NONE;

	wd = vc.next();
	while(wd.type != WordDescriptor::END) {
		addWord(bitmap, wd, status);
		wd = vc.next();
	}
	this->length = v.size();
	this->max = v.back();
}

};