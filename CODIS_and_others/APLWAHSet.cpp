#include "APLWAHSet.h"
#include <ctime>
#include <array>

void APLWAHSet::fromVector(const vector<uint32_t>& v){
		length = v.size();
		chunkMax = (v.back()/31) + 1;
		
		bitmap.clear();
		bitmap.reserve(chunkMax/4);

		int CCount = 0;
		vector<uint32_t>::const_iterator ICount = v.begin();

		vector<uint32_t>::const_iterator lastLitStart = v.begin();
		vector<uint32_t>::const_iterator lastLitEnd = v.begin();
		Word lastLit = 0;

		while(ICount != v.end()){
			int litStart = *ICount;  //本Literal的第一个数字
			int litCCount = litStart/31;
			int FillCount = litCCount - CCount;
			vector<uint32_t>::const_iterator IEnd = ICount;	//ICount当前data的指针，IEnd本Literal的最后一个数字在data中的index
			int litEnd = litCCount*31 + 31;	//本Literal的边界末尾

			Word lit = 0x80000000;

			while(IEnd != v.end() && *IEnd < litEnd){
				lit |= 1<<(*IEnd%31);
				IEnd ++;
			}

			if(lastLitStart == lastLitEnd){  // 没有预备L
				if(FillCount == 0){  // 得到L
					if(IEnd - ICount < 4){  //之后可能被压缩，作为后备L
						lastLitStart = ICount;
						lastLitEnd = IEnd;
						lastLit = lit;
					}
					else{ //不可能被压缩，直接输出
						bitmap.push_back(lit);
					}
				}
				else{ //  得到 F与L
					if(IEnd - ICount < 4){ //可能生成FL
						int n = (_lzcnt_u32(FillCount) - 5)/5;
						if(IEnd - ICount > n){ //Fill太大，不能生成FL，L留作预备
							bitmap.push_back(FillCount);
							lastLitStart = ICount;
							lastLitEnd = IEnd;
							lastLit = lit;
						}
						else{ //可以生成FL
							FillCount |= (IEnd - ICount) << 27;
							for(vector<uint32_t>::const_iterator itr = ICount;itr < IEnd;itr++){
								FillCount |= (*itr%31) << (22 - 5*(itr - ICount)) ;
							}
							bitmap.push_back(FillCount);
						}
					}
					else{  //L太脏，不能生成FL
						bitmap.push_back(FillCount);
						bitmap.push_back(lit);
					}
				}
			}
			else{ //有预备L
				if(FillCount==0){ // L+L
					if(IEnd - ICount < 4){ //后一个可能可能被压缩
						
						if(IEnd - ICount + lastLitEnd - lastLitStart < 6){ //LL 可以生成
							lit = 0;
							lit |= (lastLitEnd - lastLitStart) << 29;
							lit |= (IEnd - ICount) << 27;

							for(vector<uint32_t>::const_iterator itr = lastLitStart;itr < lastLitEnd;itr++){
								lit |= (*itr%31) << (22 - 5*(itr - lastLitStart)) ;
							}

							int countStart = (ICount - lastLitEnd) + (lastLitStart - v.begin());

							for(vector<uint32_t>::const_iterator itr = ICount;itr < IEnd;itr++){
								lit |= (*itr%31) << (22 - 5*(itr - v.begin() - countStart)) ;
							}
							bitmap.push_back(lit);
							lastLitStart = lastLitEnd;
						}
						else{  // LL不能生成，输出预备L，现有L留作预备
							bitmap.push_back(lastLit);
							lastLit = lit;
							lastLitStart = ICount;
							lastLitEnd = IEnd;
						}
					}
					else{ //现有L太脏，全输出
						bitmap.push_back(lastLit);
						bitmap.push_back(lit);
						lastLitStart = lastLitEnd;
					}
				}
				else{ // 得到L，F，L
					if(IEnd - ICount < 4){  //现有L可能被压缩
						int n = ((_lzcnt_u32(FillCount) - 5)/5) + 1;
						if(IEnd - ICount + lastLitEnd - lastLitStart < n){ // LFL可以生成
							FillCount |= (lastLitEnd - lastLitStart) << 29;
							FillCount |= (IEnd - ICount) << 27;

							for(vector<uint32_t>::const_iterator itr = lastLitStart;itr < lastLitEnd;itr++){
								FillCount |= (*itr%31) << (22 - 5*(itr - lastLitStart)) ;
							}

							int countStart = ICount - lastLitEnd + lastLitStart - v.begin();

							for(vector<uint32_t>::const_iterator itr = ICount;itr < IEnd;itr++){
								FillCount |= (*itr%31) << (22 - 5*(itr - v.begin() - countStart)) ;
							}
							bitmap.push_back(FillCount);
							lastLitStart = lastLitEnd;
						}
						else if(lastLitEnd - lastLitStart < n){ // LF 可以生成
							FillCount |= (lastLitEnd - lastLitStart) << 29;

							for(vector<uint32_t>::const_iterator itr = lastLitStart;itr < lastLitEnd;itr++){
								FillCount |= (*itr%31) << (22 - 5*(itr - lastLitStart)) ;
							}

							bitmap.push_back(FillCount);
							lastLitStart = ICount;
							lastLitEnd = IEnd;
							lastLit = lit;
						}
						else if(IEnd - ICount < n){  //FL可以生成
							bitmap.push_back(lastLit);

							FillCount |= (IEnd - ICount) << 27;


							for(vector<uint32_t>::const_iterator itr = ICount;itr < IEnd;itr++){
								FillCount |= (*itr%31) << (22 - 5*(itr - ICount)) ;
							}
							bitmap.push_back(FillCount);
							lastLitStart = lastLitEnd;
						}
						else{ //都不能生成
							bitmap.push_back(lastLit);
							bitmap.push_back(FillCount);
							lastLit = lit;
							lastLitStart = ICount;
							lastLitEnd = IEnd;
						}
					}
					else{  //现有L太脏
						bitmap.push_back(lastLit);
						bitmap.push_back(FillCount);
						bitmap.push_back(lit);
						lastLitStart = lastLitEnd;
					}
				}
			}

			CCount = litCCount + 1;
			ICount = IEnd;
		}

		if(lastLitStart != lastLitEnd){
			bitmap.push_back(lastLit);
		}


}

void APLWAHSet::intersection(const vector<APLWAHSet>& sets, vector<uint32_t>& ret){
	ret.clear();
	ret.reserve(sets[0].length/sets.size());



	WordItr* baseItr = new WordItr[sets.size()];
	for(unsigned int i=0;i<sets.size();i++){
		baseItr[i] = sets[i].bitmap.begin();
	}
	
	APLWAHDecoder* Decoders = new APLWAHDecoder[sets.size()];

	int* ChunkItr = new int[sets.size()];
	memset(ChunkItr,0,sizeof(int)*sets.size());
	
	//int lastLit = 0;

	while((!Decoders[0].isEmpty) || (baseItr[0] != sets[0].bitmap.end())){
		Word lit = 0;
		
		//grab a Literal from sets[0]
		if(Decoders[0].isEmpty){
			Word w = *baseItr[0];
			baseItr[0]++;

			if(_bittestandreset((long*)&w,31)){
				lit = w;
				ChunkItr[0]++;
			}
			else{
				Decoders[0].extractHalf(w);
				if(Decoders->len1 != 0){
					Decoders->extractL1();
					lit = Decoders->lit1;
					ChunkItr[0]++;
				}
				else if(Decoders->len2 != 0){
					ChunkItr[0] += Decoders[0].counter + 1;
					Decoders->extractL2();
					lit = Decoders->lit2;
					Decoders->clear();
				}
				else{
					ChunkItr[0] += Decoders[0].counter;
					Decoders->clear();
				}
			}
		} else{
			ChunkItr[0] += Decoders[0].counter;
			if(Decoders[0].len2 != 0){
				Decoders->extractL2();
				lit = Decoders->lit2;
				ChunkItr[0]++;
			}
			Decoders->clear();
		}

		if(lit == 0) continue;

		// a Literal has been grabbed
		for(unsigned int i=1;i<sets.size();i++){
			if(ChunkItr[i] >= ChunkItr[0]) {
				lit = 0;
				break;
			} 

			while(true){
				//上一轮有残余的LFL
				if(!Decoders[i].isEmpty){ 
					int step = Decoders[i].counter + (Decoders[i].len2  != 0);
					if(ChunkItr[i] + step > ChunkItr[0]){
						ChunkItr[i] += Decoders[i].counter;
						Decoders[i].counter = 0;
						lit = 0;
						if(Decoders[i].len2 == 0) Decoders[i].clear();
						break;
					}
					else if(ChunkItr[i] + step == ChunkItr[0]){
						ChunkItr[i]+= step;
						if(Decoders[i].len2 == 0) {
							lit = 0;
						}
						else{
							Decoders[i].extractL2();
							lit &= Decoders[i].lit2;
						}
						Decoders[i].clear();
						break;
					}
					else{
						ChunkItr[i]+=step;
						Decoders[i].clear();
					}
				}
				else if(baseItr[i] == sets[i].bitmap.end()){
					lit = 0;
					break;
				}
				else{ 
					Word sw = *(baseItr[i]);
					baseItr[i]++;
					int step = 0;
					if(_bittest((long*)&sw,31)){
						step = 1;
					}
					else{
						step = Decoders[i].extractHalf(sw);
					}

					if(ChunkItr[i] + step > ChunkItr[0]){
						if(step == 1){
							printf("Terrible Error!!\n\n\n");
						}

						//进入此分支，说明第一个Literal是与base 对齐的
						else if(Decoders[i].len1 != 0 && ((ChunkItr[i] + 1) == ChunkItr[0])){
							Decoders[i].extractL1();
							lit &= Decoders[i].lit1;
							ChunkItr[i]++;
						}

						else if(Decoders[i].len2 != 0){
							ChunkItr[i] += Decoders[i].counter + (Decoders[i].len1 != 0);
							Decoders[i].counter = 0;
							lit = 0;
						}
						else{
							ChunkItr[i] += step;
							Decoders[i].clear();
							lit = 0;
						}
						break;
					}
					else if(ChunkItr[i] + step == ChunkItr[0]){
						ChunkItr[i] += step;
						if(_bittest((long*)&sw,31)){
							lit &= sw;
						}
						else if(Decoders[i].len2 != 0){
							Decoders[i].extractL2();
							lit &= Decoders[i].lit2;
							Decoders[i].clear();
						}
						else{
							Decoders[i].clear();
							lit = 0;
						}
						break;
					}
					else{
						ChunkItr[i] += step;
						Decoders[i].clear();
					}
				}
			}
		
			if(lit == 0 || lit == 0x80000000) break;
		}
		//for 循环结束
		if(lit != 0 && lit != 0x80000000){
			int baseNum = ChunkItr[0]*31 - 31;
			_bittestandreset((long*)&lit,31);
			while(lit != 0){
				int zc = _tzcnt_u32(lit);
				_bittestandreset((long*)&lit,zc);
				ret.push_back(zc + baseNum);
			}
		}
		
	}
	delete[] baseItr;
	delete[] Decoders;
	delete[] ChunkItr;
	return;
}

void APLWAHSet::toVector(vector<uint32_t>& ret)
{
	ret.clear();
	ret.reserve(length);

#ifdef TIMING_FOR_APLWAH
	clock_t t1;
	clock_t t2;
	t1 = clock();
#endif

	WordItr itr = bitmap.begin();
	int base = 0;
	array<int,32> buff;
	while(itr != bitmap.end()){
		int chunkN = APLWAHDecoder::decodeLit((*itr),buff);
		array<int,32>::const_iterator intItr = buff.begin();
		while(*intItr != -1){
			ret.push_back(*intItr + base);
			intItr++;
		}
		base += chunkN;
		itr++;
	}
#ifdef TIMING_FOR_APLWAH
	t2 = clock();
	printf("APLWAH.toVector:\n%d integers, %d codewords, %d ms cost\n\n",length,bitmap.size(),t2 - t1);
#endif
}

int APLWAHDecoder::decodeLit(Word w, array<int,32>& plist){
	int cnt = 0;
	int ret = 0;
	if(_bittestandreset((long*)&w,31)){
		while(w != 0){
			int i = _tzcnt_u32(w);
			_bittestandreset((long*)&w,i);
			plist[cnt] = i;
			cnt++;
		}
		ret = 31;
	}
	else{
		int len1 = _bextr_u32(w,29,2);
		int len2 = _bextr_u32(w,27,2);
		
		int counter = _bextr_u32(w,0,27 - 5*(len1 + len2));
		int pos = 22;

		if(len1 != 0){
			switch(len1){
			case 3:
				plist[cnt] = (ret + _bextr_u32(w,pos,5));
				cnt++;
				pos -= 5;
			case 2:
				plist[cnt] = (ret + _bextr_u32(w,pos,5));
				cnt++;
				pos -= 5;
			case 1:
				plist[cnt] = (ret + _bextr_u32(w,pos,5));
				cnt++;
				pos -= 5;
			}
			ret +=31;
		}

		ret += counter*31;

		if(len2 != 0){
			
			switch(len2){
			case 3:
				plist[cnt] = (ret + _bextr_u32(w,pos,5));
				cnt++;
				pos -= 5;
			case 2:
				plist[cnt] = (ret + _bextr_u32(w,pos,5));
				cnt++;
				pos -= 5;
			case 1:
				plist[cnt] = (ret + _bextr_u32(w,pos,5));
				cnt++;
				pos -= 5;
			}
			ret+=31;
		}

		
	}

	plist[cnt] = -1;
	return ret;
}

int APLWAHDecoder::extractHalf(const Word& w)
{
	len1 = _bextr_u32(w,29,2);
	len2 = _bextr_u32(w,27,2);
	int CounterLen =5*(len1 + len2);
	counter = _bextr_u32(w,0,27 - CounterLen);
	lit = _bextr_u32(w,27 - CounterLen,CounterLen);
	isEmpty = false;
	return counter + (len1 != 0) + (len2 != 0);
}



void APLWAHDecoder::extractL1()
{
	int pos = len2*5;
	lit1 = 0x80000000;
	switch(len1){
	case 3:
		lit1 |= (0x1<<_bextr_u32(lit,pos,5));
		pos+=5;
	case 2:
		lit1 |= (0x1<<_bextr_u32(lit,pos,5));
		pos+=5;
	case 1:
		lit1 |= (0x1<<_bextr_u32(lit,pos,5));
		pos+=5;
	case 0:
		;
	}
}


void APLWAHDecoder::extractL2()
{
	int pos = 0;
	lit2 = 0x80000000;
	switch(len2){
	case 3:
		lit2 |= (0x1<<_bextr_u32(lit,pos,5));
		pos+=5;
	case 2:
		lit2 |= (0x1<<_bextr_u32(lit,pos,5));
		pos+=5;
	case 1:
		lit2 |= (0x1<<_bextr_u32(lit,pos,5));
		pos+=5;
	case 0:
		;
	}
}
