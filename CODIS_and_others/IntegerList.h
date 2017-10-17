#ifndef __INTEGER_LIST__
#define __INTEGER_LIST__
#include <vector>

class IntegerList{
public:
	vector<uint32_t> buff;

	IntegerList(){;}

	void fromVector(const vector<uint32_t>& v){
		buff = v;
	}


	inline IntegerList(const vector<uint32_t>& v){fromVector(v);}

	static void intersection(const vector<IntegerList>& sets, vector<uint32_t>& ret){
		if(sets.size()<=1) return;
		ret.resize(sets[0].buff.size());
		vector<uint32_t>::iterator it = set_intersection(sets[0].buff.begin(),sets[0].buff.end(),sets[1].buff.begin(),sets[1].buff.end(),ret.begin());
		for(int i=2;i<sets.size();i++){
			it = set_intersection(sets[i].buff.begin(),sets[i].buff.end(),ret.begin(),it,ret.begin());
		}
		ret.erase(it,ret.end());
		return;
	}

	inline double getBpi() const{return sizeof(int);}

	inline ~IntegerList(){;}

	void toVector(vector<uint32_t>& ret){
		ret = buff;
	}
};


#endif


