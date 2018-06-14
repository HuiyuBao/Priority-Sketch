#ifndef _COUNTINGBF_H
#define _COUNTINGBF_H

#include <algorithm>
#include <cstring>
#include <string.h>
#include "priority.h"
#include "BOBHash.h"
#include <iostream>

using namespace std;

class CountingBF
{
private:
    BOBHash *bobhash[MAX_HASH_NUM];
    int index[MAX_HASH_NUM];
    int *counter;
    int w;
    int MAX_CNT;
    llu hash_value;

public:
    CountingBF(int _w)
    {
        w=_w;
        counter=new int[w];
        memset(counter,0,w*sizeof(int));
        MAX_CNT=(1<<COUNTER_SIZE)-1;
        for(int i=0;i<LOW_HASH_NUM;i++)
            bobhash[i]=new BOBHash(i+1000);
    }
    void Insert(const char *str)
    {
        for(int i=0;i<LOW_HASH_NUM;i++)
        {
            index[i]=(bobhash[i]->run(str,strlen(str)))%w;
            if(counter[index[i]]!=MAX_CNT)
                counter[index[i]]++;
        }
    }
    int Query(const char *str)
    {
        int min_value=MAX_CNT;
        int temp;

        for(int i=0;i<LOW_HASH_NUM;i++)
        {
            index[i]=(bobhash[i]->run(str,strlen(str)))%w;
            temp=counter[index[i]];
            min_value=temp<min_value?temp:min_value;
        }
        return min_value;
    }
    void Delete(const char *str)
    {
        for(int i=0;i<LOW_HASH_NUM;i++)
        {
            index[i]=(bobhash[i]->run(str,strlen(str)))%w;
            counter[index[i]]--;
        }
    }
    ~CountingBF()
    {
        delete []counter;
        for(int i=0;i<LOW_HASH_NUM;i++)
            delete bobhash[i];
    }
};
#endif
