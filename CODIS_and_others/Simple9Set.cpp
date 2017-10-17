#include "Simple9Set.h"
#include <array>
#include <intrin.h>



void Simple9Set::decode0(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+w);
}

void Simple9Set::decode1(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+_bextr_u32(w,14,14));
	v.push_back(v.back()+_bextr_u32(w,0,14));
}

void Simple9Set::decode2(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+_bextr_u32(w,18,9));
	v.push_back(v.back()+_bextr_u32(w,9,9));
	v.push_back(v.back()+_bextr_u32(w,0,9));
}

void Simple9Set::decode3(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+_bextr_u32(w,21,7));
	v.push_back(v.back()+_bextr_u32(w,14,7));
	v.push_back(v.back()+_bextr_u32(w,7,7));
	v.push_back(v.back()+_bextr_u32(w,0,7));
}

void Simple9Set::decode4(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+_bextr_u32(w,20,5));
	v.push_back(v.back()+_bextr_u32(w,15,5));
	v.push_back(v.back()+_bextr_u32(w,10,5));
	v.push_back(v.back()+_bextr_u32(w,5,5));
	v.push_back(v.back()+_bextr_u32(w,0,5));
}

void Simple9Set::decode5(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+_bextr_u32(w,24,4));
	v.push_back(v.back()+_bextr_u32(w,20,4));
	v.push_back(v.back()+_bextr_u32(w,16,4));
	v.push_back(v.back()+_bextr_u32(w,12,4));
	v.push_back(v.back()+_bextr_u32(w,8,4));
	v.push_back(v.back()+_bextr_u32(w,4,4));
	v.push_back(v.back()+_bextr_u32(w,0,4));
}

void Simple9Set::decode6(uint32_t& w,vector<uint32_t>& v)
{
	v.push_back(v.back()+_bextr_u32(w,24,3));
	v.push_back(v.back()+_bextr_u32(w,21,3));
	v.push_back(v.back()+_bextr_u32(w,18,3));
	v.push_back(v.back()+_bextr_u32(w,15,3));
	v.push_back(v.back()+_bextr_u32(w,12,3));
	v.push_back(v.back()+_bextr_u32(w,9,3));
	v.push_back(v.back()+_bextr_u32(w,6,3));
	v.push_back(v.back()+_bextr_u32(w,3,3));
	v.push_back(v.back()+_bextr_u32(w,0,3));
}

void Simple9Set::decode7(uint32_t& w,vector<uint32_t>& v)
{
	for(int i=26;i>= 0;i-=2){
		v.push_back(v.back()+_bextr_u32(w,i,2));
	}
}

void Simple9Set::decode8(uint32_t& w,vector<uint32_t>& v)
{
	for(int i=0;i<28;i++){
		v.push_back(v.back()+1);
	}
}



void Simple9Set::initDecoders()
{
	decoder[0] = decode0;
	decoder[1] = decode1;
	decoder[2] = decode2;
	decoder[3] = decode3;
	decoder[4] = decode4;
	decoder[5] = decode5;
	decoder[6] = decode6;
	decoder[7] = decode7;
	decoder[8] = decode8;
}

void Simple9Set::fromVector(const vector<uint32_t>& v)
{
	base = v[0];
	cnt = v.size();
	buff.clear();

	vector<uint32_t> tmp;
	tmp.reserve(28);
	int lastSolution = -1;
	int maxBit = 0;
	const int lengthToSolution[33] = {-1,0,1,2,3,4,4,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8};
	const int solutionToLength[9] = {1,2,3,4,5,7,9,14,28};
	const int solutionToBits[9] = {28,14,9,7,5,4,3,2,1};

	for(int i=1;i<v.size();i++){
		tmp.push_back( v[i] - v[i - 1]);
		int bits = 32 - _lzcnt_u32(tmp.back());
		maxBit = maxBit > bits? maxBit:bits;
		if(maxBit*tmp.size() > 28){
			uint32_t w = lastSolution << 28;
			for(int j=0;j<solutionToLength[lastSolution];j++){
				w |= tmp[j]<<((solutionToLength[lastSolution]-j-1)*solutionToBits[lastSolution]);
			}
			buff.push_back(w);
			i -= (tmp.size()-solutionToLength[lastSolution]);
			tmp.clear();
			maxBit = 0;
		}
		else{
			lastSolution = lengthToSolution[tmp.size()];
		}
	}


	if(tmp.size() != 0){
		for(int k = 0;k<tmp.size();k++){
			buff.push_back(tmp[k]);
		}
	}
	
}




void Simple9Set::toVector(vector<uint32_t>& ret) const
{
	ret.clear();
	ret.reserve(cnt);
	ret.push_back(base);
	for(int i=0;i<buff.size();i++){
		uint32_t W = buff[i];
		decoder[_bextr_u32(W,28,4)](W,ret);
	}
}

void Simple9Set::intersection(const vector<Simple9Set>& sets, vector<uint32_t>& ret)
{
	if(sets.size()<=1) return;
	
	vector<uint32_t> v1;
	sets[0].toVector(v1);
	sets[1].toVector(ret);

	vector<uint32_t>::iterator it = set_intersection(v1.begin(),v1.end(),ret.begin(),ret.end(),ret.begin());
	for(int i=2;i<sets.size();i++){
		sets[i].toVector(v1);
		it = set_intersection(v1.begin(),v1.end(),ret.begin(),it,ret.begin());
	}
	ret.erase(it,ret.end());
	return;
}

int Simple9Set::findIthNumbersBlock(int& index)
{
	vector<uint32_t> ret;
	ret.reserve(cnt);
	ret.push_back(base);
	for(int i=0;i<buff.size();i++){
		uint32_t W = buff[i];
		decoder[_bextr_u32(W,28,4)](W,ret);
		if(ret.size() >= index){
			index = ret.size();
			printf("Simple9 Check: Type %d\n",_bextr_u32(W,28,4));
			return i;
		}
	}
}
