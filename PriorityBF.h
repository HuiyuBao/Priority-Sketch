#ifndef _PRIORITYBF_H
#define _PRIORITYBF_H

#include <algorithm>
#include <cstring>
#include <string.h>
#include "priority.h"
#include "BOBHash.h"
#include <iostream>

using namespace std;

class PriorityBF
{
private:
    BOBHash *bobhash[MAX_HASH_NUM];
    int index[MAX_HASH_NUM];
    int *counter;
    int w,pri_num;
    int MAX_CNT;
    llu hash_value;

public:
    PriorityBF(int _w,int _pri_num)
    {
        w=_w; pri_num=_pri_num;
        counter=new int[w];
        memset(counter,0,w*sizeof(int));
        MAX_CNT=(1<<COUNTER_SIZE)-1;
        for(int i=0;i<LOW_HASH_NUM+pri_num-1;i++)
            bobhash[i]=new BOBHash(i+1000);
    }
    void Insert(const char *str,int pri)
    {
        for(int i=0;i<LOW_HASH_NUM+pri;i++)
        {
            index[i]=(bobhash[i]->run(str,strlen(str)))%w;
            if(counter[index[i]]!=MAX_CNT)
                counter[index[i]]++;
        }
    }
    int Query(const char *str,int pri)
    {
        int min_value=MAX_CNT;
        int temp;

        for(int i=0;i<LOW_HASH_NUM+pri;i++)
        {
            index[i]=(bobhash[i]->run(str,strlen(str)))%w;
            temp=counter[index[i]];
            min_value=temp<min_value?temp:min_value;
        }
        return min_value;
    }
    void Delete(const char *str,int pri)
    {
        for(int i=0;i<LOW_HASH_NUM+pri;i++)
        {
            index[i]=(bobhash[i]->run(str,strlen(str)))%w;
            counter[index[i]]--;
        }
    }
    ~PriorityBF()
    {
        delete []counter;
        for(int i=0;i<LOW_HASH_NUM+pri_num-1;i++)
            delete bobhash[i];
    }
};
#endif
/*
类似bloomfilter的数据结构
优先级越高的数据使用更多的hash函数，0优先级用4个，7优先级用11个
然后查询时，寻找最小的格子
支持删除操作
*/
