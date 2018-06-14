#ifndef _PRIORITYCM_H
#define _PRIORITYCM_H

#include <algorithm>
#include <cstring>
#include <string.h>
#include "priority.h"
#include "BOBHash.h"
#include <iostream>

using namespace std;

class PriorityCM
{
private:
  	BOBHash *bobhash[MAX_HASH_NUM];
  	int index[MAX_HASH_NUM];
  	int *counter[MAX_HASH_NUM];
  	int w,pri_num;
    int counter_num;
  	int MAX_CNT;

public:
  	PriorityCM(int _w,int _pri_num)
  	{
  		  w=_w; pri_num=_pri_num;
        counter_num=LOW_HASH_NUM+pri_num-1;

    		for(int i=0;i<counter_num;i++)
    		{
      			counter[i]=new int[w];
      			memset(counter[i],0,sizeof(int)*w);
    		}
        MAX_CNT = (1 << COUNTER_SIZE) - 1;

    		for(int i=0;i<counter_num;i++)
      	   bobhash[i] = new BOBHash(i + 1000);
  	}
  	void Insert(const char * str,int pri)
  	{
        //int from=(bobhash[counter_num]->run(str,strlen(str)))%counter_num;
        uint tmp=bobhash[0]->run(str,strlen(str));
        int from=((tmp>>24)&0xFF)%counter_num;
        index[0]=(tmp&0xFFFFFF)%w;
        //int to=from+LOW_HASH_NUM+pri-1;

        for(int i=0;i<LOW_HASH_NUM+pri;i++)
        {
            if(from!=0)
                index[from]=(bobhash[from]->run(str,strlen(str)))%w;
            if(counter[from][index[from]]!=MAX_CNT)
                counter[from][index[from]]++;

            from++;
            if(from>=counter_num)from-=counter_num;
        }
  	}
    int Query(const char *str,int pri)
    {
        //int from=(bobhash[counter_num]->run(str,strlen(str)))%counter_num;
        uint tmp=bobhash[0]->run(str,strlen(str));
        int from=((tmp>>24)&0xFF)%counter_num;
        index[0]=(tmp&0xFFFFFF)%w;
        int min_value=MAX_CNT;
        int temp;

        for(int i=0;i<LOW_HASH_NUM+pri;i++)
        {
            if(from!=0)
                index[from]=(bobhash[i]->run(str,strlen(str)))%w;
            temp=counter[from][index[from]];
            min_value=temp<min_value?temp:min_value;

            from++;
            if(from>=counter_num)from-=counter_num;
        }
        return min_value;
    }
    void Delete(const char * str,int pri)
  	{
        //int from=(bobhash[counter_num]->run(str,strlen(str)))%counter_num;
        uint tmp=bobhash[0]->run(str,strlen(str));
        int from=((tmp>>24)&0xFF)%counter_num;
        index[0]=(tmp&0xFFFFFF)%w;

        for(int i=0;i<LOW_HASH_NUM+pri;i++)
        {
            if(from!=0)
                index[from]=(bobhash[i]->run(str,strlen(str)))%w;
            counter[from][index[from]]--;

            from++;
            if(from>=counter_num)from-=counter_num;
        }
    }
  	~PriorityCM()
  	{
    		for(int i=0;i<counter_num;i++)
    			 delete []counter[i];

        for(int i=0;i<=counter_num;i++)
    			delete bobhash[i];
  	}
};
#endif//_PriorityCM_H
