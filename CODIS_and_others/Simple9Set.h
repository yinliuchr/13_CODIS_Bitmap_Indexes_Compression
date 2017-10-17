#ifndef __SIMPLE9SET__
#define __SIMPLE9SET__
#include <cstdint>
#include <vector>
using namespace std;

class Simple9Set{

public:
	vector<uint32_t> buff;
	typedef void (*Decoder)(uint32_t& w,vector<uint32_t>& v);
	Decoder decoder[9];
	static void decode0(uint32_t& w,vector<uint32_t>& v);
	static void decode1(uint32_t& w,vector<uint32_t>& v);
	static void decode2(uint32_t& w,vector<uint32_t>& v);
	static void decode3(uint32_t& w,vector<uint32_t>& v);
	static void decode4(uint32_t& w,vector<uint32_t>& v);
	static void decode5(uint32_t& w,vector<uint32_t>& v);
	static void decode6(uint32_t& w,vector<uint32_t>& v);
	static void decode7(uint32_t& w,vector<uint32_t>& v);
	static void decode8(uint32_t& w,vector<uint32_t>& v);

	int base;
	int cnt;
	void initDecoders();
	Simple9Set(){cnt = 0;base = 0;initDecoders();buff.clear();}
	inline Simple9Set(const vector<uint32_t>& v){fromVector(v);initDecoders();}
	void fromVector(const vector<uint32_t>& v);
	void toVector (vector<uint32_t>& ret)const;
	static void intersection(const vector<Simple9Set>& sets, vector<uint32_t>& ret);
	inline double getBpi() const{return 32.0*buff.size()/cnt;};
	void clear(){buff.clear(); base = 0; cnt = 0;}
	int findIthNumbersBlock(int& index);
};


#endif