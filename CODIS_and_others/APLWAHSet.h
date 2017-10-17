#ifndef __APLWAHSET__
#define __APLWAHSET__
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <intrin.h>
#include <array>
using namespace std;

typedef uint32_t Word;
typedef vector<Word> Bitmap;
typedef vector<Word>::const_iterator WordItr;


class APLWAHDecoder{
public:
	bool isEmpty;
	unsigned char len1;
	unsigned char len2;
	int counter;
	Word lit1;
	Word lit2;
	Word lit;
	APLWAHDecoder(){isEmpty = true;}

	static int decodeLit(Word w, array<int,32>& plist);
	int extractHalf(const Word& w);
	void extractL1();
	void extractL2();
	inline void clear(){isEmpty = true;}
};



class APLWAHSet{
//#define TIMING_FOR_APLWAH
public:
	Bitmap bitmap; //���ڴ洢bitmap��vector
	int length;		//��set���м�������
	int chunkMax;	//bitmap�е�chunk�����Ϊ����
	
	inline APLWAHSet(){
		length = 0;
		chunkMax = 0;
	}

	
	void fromVector(const vector<uint32_t>& v); //v �Ǵ�С��������������б���v����λͼ
	

	inline APLWAHSet(const vector<uint32_t>& v){fromVector(v);}
	
	static void intersection(const vector<APLWAHSet>& sets, vector<uint32_t>& ret); //������ϵĽ���

	inline double getBpi() const{return bitmap.size()/(length+0.0);}
		
	inline ~APLWAHSet(){;}
	
	void toVector(vector<uint32_t>& ret);
	void clear(){bitmap.clear(); length = 0; chunkMax = 0;}
};


#endif