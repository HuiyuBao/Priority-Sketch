#ifndef _PRIORITYC_H
#define _PRIORITYC_H

#include <algorithm>
#include <cstring>
#include <string.h>
#include "priority.h"
#include "BOBHash.h"
#include <iostream>
#include <algorithm>
using namespace std;

class PriorityC
{	
private:
	BOBHash * bobhash[MAX_HASH_NUM * 2];
	int index[MAX_HASH_NUM];
	int *counter[MAX_HASH_NUM];
	int w, pri_num;
	int MAX_CNT, MIN_CNT;
	int counter_num;
	uint64_t hash_value;

public:
	PriorityC(int _w, int _pri_num)
	{
		w = _w;
		pri_num = _pri_num;
        counter_num = LOW_HASH_NUM + pri_num - 1;
		
		for(int i = 0; i < counter_num; i++)	
		{
			counter[i] = new int[w];
			memset(counter[i], 0, sizeof(int) * w);
		}

		MAX_CNT = (1 << (COUNTER_SIZE - 1)) - 1;
		MIN_CNT = (-(1 << (COUNTER_SIZE - 1)));

		for(int i = 0; i < counter_num * 2; i++)
		{
			bobhash[i] = new BOBHash(i + 1000);
		}
	}
	void Insert(const char * str,int pri)
	{
		int g = 0;
        uint tmp=bobhash[0]->run(str,strlen(str));
        int from=((tmp>>24)&0xFF)%counter_num;
        index[0]=(tmp&0xFFFFFF)%w;

        for(int i = 0;i < LOW_HASH_NUM + pri;i++)
        {
            if(from != 0)
                index[from]=(bobhash[from]->run(str,strlen(str)))%w;
            
            g = (bobhash[from+counter_num]->run(str,strlen(str)))%2;

            if(g == 0)
            {
                if(counter[from][index[from]]!=MAX_CNT)
                    counter[from][index[from]]++;
            }
            else
            {
                if(counter[from][index[from]]!=MIN_CNT)
                    counter[from][index[from]]--;
            }
            from++;
            if(from>=counter_num)from-=counter_num;
        }

	}
	void Delete(const char * str,int pri)
	{
		int g = 0;
        uint tmp=bobhash[0]->run(str,strlen(str));
        int from=((tmp>>24)&0xFF)%counter_num;
        index[0]=(tmp&0xFFFFFF)%w;

		for(int i = 0; i < LOW_HASH_NUM + pri; i++)
		{
			
            if(from != 0)
                index[from] = (bobhash[from]->run(str,strlen(str)))%w;

			g = (bobhash[from + pri_num]->run(str, strlen(str))) % 2;

			if(g == 1)
			{
                if(counter[from][index[from]] != MAX_CNT)
                    counter[from][index[from]]++;
			}
			else
			{
                if(counter[from][index[from]] != MIN_CNT)
                    counter[from][index[from]]--;
			}

            from++;
            if(from>=pri_num)from-=pri_num;
		}
	}
	int Query(const char *str,int pri)
	{
		int temp;
		int res[MAX_HASH_NUM];
		int g;

        uint tmp=bobhash[0]->run(str,strlen(str));
        int from=((tmp>>24)&0xFF)%counter_num;
        index[0]=(tmp&0xFFFFFF)%w;

		for(int i = 0; i < LOW_HASH_NUM + pri; i++)
		{
            if(from!=0)index[from]=(bobhash[from]->run(str,strlen(str)))%w;
            temp=counter[from][index[from]];
            g=(bobhash[from+pri_num]->run(str,strlen(str)))%2;
            res[i]=(g==0?temp:-temp);

            from++;
            if(from>=pri_num)from-=pri_num;
		}

        int len=LOW_HASH_NUM+pri;
        sort(res,res+len);
        int r;
        if(len%2==0)
            r=(res[len/2]+res[len/2-1])/2;
        else
            r=res[len/2];
        return r;
	}
	~PriorityC()
	{
		for(int i = 0; i < counter_num; i++)	
			delete []counter[i];

		for(int i = 0; i < counter_num * 2; i++)
			delete bobhash[i];
	}
};
#endif//_PriorityC_H