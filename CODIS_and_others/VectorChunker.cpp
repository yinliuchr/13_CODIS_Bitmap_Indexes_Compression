#include "VectorChunker.h"

WordDescriptor VectorChunker::next() {
	WordDescriptor wd;
	if (this->itr == this->integers->end()) {
		wd.type = WordDescriptor::END;
	}
	else {
		this->pointer += 31;	

		if (*itr >= this->pointer) {
			wd.type = WordDescriptor::ALL_ZERO;
		}
		else {
			wd.type = WordDescriptor::LITERAL;
			wd.word = 0x80000000;
			
			while(this->itr != this->integers->end() && *(this->itr) < this->pointer) {
				_bittestandset((long*)&(wd.word), *(this->itr) - this->pointer + 31);
				this->itr++;
			}

			if(wd.word == 0xFFFFFFFF) {
				wd.type = WordDescriptor::ALL_ONE;
			}
		}
	}

	return wd;
}