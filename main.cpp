#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <string.h>
#include <ctime>
#include <time.h>
#include <iterator>
#include <math.h>
#include <vector>

#include "PriorityBF.h"
#include "CountingBF.h"
#include "PriorityCM.h"
#include "CMSketch.h"
#include "PriorityCU.h"
#include "CUSketch.h"

#define testcycles 10
#define PRI_NUM 8

using namespace std;

//char *file_stream="small-flow.txt";
char *file_stream="flows.txt";

struct comb
{
    char ss[100];
    int pri;
};

comb insert[MAX_INSERT_PACKAGE + MAX_INSERT_PACKAGE/5];
comb query[MAX_INSERT_PACKAGE + MAX_INSERT_PACKAGE/5];

unordered_map<string, int> unmp;
unordered_map<string, int> ufpri;

int main(int argc,char **argv)
{
    if(argc==2)
        file_stream=argv[1];

    unmp.clear();
    int val;

    llu temp1,temp2;
    int pri;

    double memory = 0.13;//MB
    int word_size = 64;

    int w = memory * 1024 * 1024 * 8.0 / COUNTER_SIZE;
    int w_p = memory * 1024 * 1024 * 8.0 / (word_size * 2);

    int package_num = 0;

    ifstream ifs(file_stream,ios::binary);
    while(!ifs.eof())
    {
        ifs.read((char *)&temp1,sizeof(llu));
        if(ifs.eof())break;
        ifs.read((char *)&temp2,sizeof(llu));

        sprintf(insert[package_num].ss,"%llu%llu",temp1,temp2);
        pri=(int)(temp2>>56)&0x7;
        insert[package_num].pri=pri;

        unmp[string(insert[package_num].ss)]++;
        ufpri[string(insert[package_num].ss)]=pri;
        package_num++;

        if(package_num==MAX_INSERT_PACKAGE)
            break;
    }

    printf("****************************************************\n");

    printf("dataset name: %s\n", file_stream);
    printf("total stream size = %d\n", package_num);
    printf("distinct item number = %d\n", unmp.size());

    int max_freq = 0;
    unordered_map<string, int>::iterator it=unmp.begin();

    for(int i=0;i<unmp.size();i++,it++)
    {
        strcpy(query[i].ss, it->first.c_str());
        query[i].pri=ufpri[it->first];

        int temp2=it->second;
        max_freq=max_freq>temp2?max_freq:temp2;
    }
    printf("max_freq = %d\n",max_freq);

    printf("\n*****************************************\n");

    PriorityBF *prioritybf;
    CountingBF *countingbf;

    PriorityCM *prioritycm;
    CMSketch *cmsketch;

    PriorityCU *prioritycu;
    CUSketch *cusketch;

/**************************************insert**************************************/

    timespec time1,time2;
    long long resns;



    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
    {
        prioritybf=new PriorityBF(w,PRI_NUM);
        for(int i=0;i<package_num;i++)
            prioritybf->Insert(insert[i].ss,insert[i].pri);
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);

    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_PBF = (double)1000.0 * testcycles * package_num / resns;
    printf("throughput of PriorityBloomFilter (insert): %.6lf Mips\n", throughput_PBF);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
    {
        countingbf=new CountingBF(w);
        for(int i=0;i<package_num;i++)
            countingbf->Insert(insert[i].ss);
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);

    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_CBF = (double)1000.0 * testcycles * package_num / resns;
    printf("throughput of CountingBloomFilter (insert): %.6lf Mips\n\n", throughput_CBF);





    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
    {
        prioritycm=new PriorityCM(w/(LOW_HASH_NUM+PRI_NUM-1),PRI_NUM);
        for(int i=0;i<package_num;i++)
            prioritycm->Insert(insert[i].ss,insert[i].pri);
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);

    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_PCM = (double)1000.0 * testcycles * package_num / resns;
    printf("throughput of PriorityCMSketch (insert): %.6lf Mips\n", throughput_PCM);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
    {
        cmsketch=new CMSketch(w/LOW_HASH_NUM,LOW_HASH_NUM);
        for(int i=0;i<package_num;i++)
            cmsketch->Insert(insert[i].ss);
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);

    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_CM = (double)1000.0 * testcycles * package_num / resns;
    printf("throughput of CMSketch (insert): %.6lf Mips\n\n", throughput_CM);





    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
    {
        prioritycu=new PriorityCU(w/(LOW_HASH_NUM+PRI_NUM-1),PRI_NUM);
        for(int i=0;i<package_num;i++)
            prioritycu->Insert(insert[i].ss,insert[i].pri);
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);

    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_PCU = (double)1000.0 * testcycles * package_num / resns;
    printf("throughput of PriorityCUSketch (insert): %.6lf Mips\n", throughput_PCU);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
    {
        cusketch=new CUSketch(w/LOW_HASH_NUM,LOW_HASH_NUM);
        for(int i=0;i<package_num;i++)
            cusketch->Insert(insert[i].ss);
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);

    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double throughput_CU = (double)1000.0 * testcycles * package_num / resns;
    printf("throughput of CUSketch (insert): %.6lf Mips\n\n", throughput_CU);


    printf("\n*****************************************\n");

/***********************************query******************************************/

    int res_tmp=0;
    int flow_num = unmp.size();
    int sum = 0;



    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
        for(int i=0;i<flow_num;i++)
            res_tmp=prioritybf->Query(query[i].ss,query[i].pri);
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput_PBF = (double)1000.0 * testcycles * flow_num / resns;
    printf("throughput of PriorityBloomFilter (query): %.6lf Mips\n", throughput_PBF);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
        for(int i=0;i<flow_num;i++)
            res_tmp=countingbf->Query(query[i].ss);
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput_CBF = (double)1000.0 * testcycles * flow_num / resns;
    printf("throughput of CountingBloomFilter (query): %.6lf Mips\n\n", throughput_CBF);




    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
        for(int i=0;i<flow_num;i++)
            res_tmp=prioritycm->Query(query[i].ss,query[i].pri);
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput_PCM = (double)1000.0 * testcycles * flow_num / resns;
    printf("throughput of PriorityCMSketch (query): %.6lf Mips\n", throughput_PCM);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
        for(int i=0;i<flow_num;i++)
            res_tmp=cmsketch->Query(query[i].ss);
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput_CM = (double)1000.0 * testcycles * flow_num / resns;
    printf("throughput of CMSketch (query): %.6lf Mips\n\n", throughput_CM);



    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
        for(int i=0;i<flow_num;i++)
            res_tmp=prioritycu->Query(query[i].ss,query[i].pri);
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput_PCU = (double)1000.0 * testcycles * flow_num / resns;
    printf("throughput of PriorityCUSketch (query): %.6lf Mips\n", throughput_PCU);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int t=0;t<testcycles;t++)
        for(int i=0;i<flow_num;i++)
            res_tmp=cusketch->Query(query[i].ss);
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    throughput_CU = (double)1000.0 * testcycles * flow_num / resns;
    printf("throughput of CUSketch (query): %.6lf Mips\n\n", throughput_CU);

    printf("\n*****************************************\n");

/**********************************aae,are*****************************************/


    char temp[500];

    double re_pbf = 0.0, re_cbf = 0.0, re_pcm = 0.0, re_cm = 0.0;
    double re_pcu=0.0,re_cu=0.0;
    double re_pbf_sum[PRI_NUM], re_cbf_sum[PRI_NUM], re_pcm_sum[PRI_NUM], re_cm_sum[PRI_NUM];
    double re_pcu_sum[PRI_NUM],re_cu_sum[PRI_NUM];
    memset(re_pbf_sum,0,sizeof(re_pbf_sum));
    memset(re_cbf_sum,0,sizeof(re_cbf_sum));
    memset(re_pcm_sum,0,sizeof(re_pcm_sum));
    memset(re_cm_sum,0,sizeof(re_cm_sum));
    memset(re_pcu_sum,0,sizeof(re_pcu_sum));
    memset(re_cu_sum,0,sizeof(re_cu_sum));

    double ae_pbf = 0.0, ae_cbf = 0.0, ae_pcm=0.0,ae_cm=0.0;
    double ae_pcu=0.0,ae_cu=0.0;
    double ae_pbf_sum[PRI_NUM], ae_cbf_sum[PRI_NUM],ae_pcm_sum[PRI_NUM],ae_cm_sum[PRI_NUM];
    double ae_pcu_sum[PRI_NUM],ae_cu_sum[PRI_NUM];
    memset(ae_pbf_sum,0,sizeof(ae_pbf_sum));
    memset(ae_cbf_sum,0,sizeof(ae_cbf_sum));
    memset(ae_pcm_sum,0,sizeof(ae_pcm_sum));
    memset(ae_cm_sum,0,sizeof(ae_cm_sum));
    memset(ae_pcu_sum,0,sizeof(ae_pcu_sum));
    memset(ae_cu_sum,0,sizeof(ae_cu_sum));

    double val_pbf = 0.0, val_cbf = 0.0, val_pcm=0.0, val_cm=0.0, val_pcu=0.0, val_cu=0.0;

    for(unordered_map<string, int>::iterator it = unmp.begin(); it != unmp.end(); it++)
    {
        strcpy(temp, (it->first).c_str());
        pri = ufpri[it->first];
        val = it->second;

        val_pbf = prioritybf->Query(temp,pri);
        val_cbf = countingbf->Query(temp);
        val_pcm = prioritycm->Query(temp,pri);
        val_cm = cmsketch->Query(temp);
        val_pcu = prioritycu->Query(temp,pri);
        val_cu = cusketch->Query(temp);


        re_pbf = fabs(val_pbf - val) / (val * 1.0);
        re_cbf = fabs(val_cbf - val) / (val * 1.0);
        re_pcm = fabs(val_pcm - val) / (val * 1.0);
        re_cm = fabs(val_cm - val) / (val * 1.0);
        re_pcu = fabs(val_pcu - val) / (val * 1.0);
        re_cu = fabs(val_cu - val) / (val * 1.0);

        ae_pbf = fabs(val_pbf - val);
        ae_cbf = fabs(val_cbf - val);
        ae_pcm = fabs(val_pcm - val);
        ae_cm = fabs(val_cm - val);
        ae_pcu = fabs(val_pcu - val);
        ae_cu = fabs(val_cu - val);


        /*re_pbf_sum += (pri*1.0)/PRI_NUM*re_pbf;
        re_cbf_sum += (pri*1.0)/PRI_NUM*re_cbf;

        ae_pbf_sum += (pri*1.0)/PRI_NUM*ae_pbf;
        ae_cbf_sum += (pri*1.0)/PRI_NUM*ae_cbf;*/

        /*re_pbf_sum += (pri+1.0)/PRI_NUM*re_pbf;
        re_cbf_sum += (pri+1.0)/PRI_NUM*re_cbf;

        ae_pbf_sum += (pri+1.0)/PRI_NUM*ae_pbf;
        ae_cbf_sum += (pri+1.0)/PRI_NUM*ae_cbf;*/

        re_pbf_sum[pri] += re_pbf;
        re_cbf_sum[pri] += re_cbf;
        re_pcm_sum[pri] += re_pcm;
        re_cm_sum[pri] += re_cm;
        re_pcu_sum[pri] += re_pcu;
        re_cu_sum[pri] += re_cu;

        ae_pbf_sum[pri] += ae_pbf;
        ae_cbf_sum[pri] += ae_cbf;
        ae_pcm_sum[pri] += ae_pcm;
        ae_cm_sum[pri] += ae_cm;
        ae_pcu_sum[pri] += ae_pcu;
        ae_cu_sum[pri] += ae_cu;
    }


    double a = package_num * 1.0;
    double b[MAX_HASH_NUM];
    memset(b,0,sizeof(b));
    for(unordered_map<string, int>::iterator it = unmp.begin(); it != unmp.end(); it++)
        b[ufpri[it->first]]++;

    for(int i=0;i<PRI_NUM;i++)
    {
        printf("for the elements with a priority of %d\n",i);
        printf("    aae_pbf = %lf\n",ae_pbf_sum[i] / b[i]);
        printf("    aae_cbf = %lf\n\n",ae_cbf_sum[i] / b[i]);

        printf("    aae_pcm = %lf\n",ae_pcm_sum[i] / b[i]);
        printf("    aae_cm  = %lf\n\n",ae_cm_sum[i] / b[i]);

        printf("    aae_pcu = %lf\n",ae_pcu_sum[i] / b[i]);
        printf("    aae_cu  = %lf\n\n",ae_cu_sum[i] / b[i]);
    }

    /*printf("aae_pbf = %lf\n", ae_pbf_sum / b);
    printf("aae_cbf = %lf\n", ae_cbf_sum / b);*/

    printf("\n*****************************************\n");

    for(int i=0;i<PRI_NUM;i++)
    {
        printf("for the elements with a priority of %d\n",i);
        printf("    are_pbf = %lf\n",re_pbf_sum[i] / b[i]);
        printf("    are_cbf = %lf\n\n",re_cbf_sum[i] / b[i]);

        printf("    are_pcm = %lf\n",re_pcm_sum[i] / b[i]);
        printf("    are_cm  = %lf\n\n",re_cm_sum[i] / b[i]);

        printf("    are_pcu = %lf\n",re_pcu_sum[i] / b[i]);
        printf("    are_cu  = %lf\n\n",re_cu_sum[i] / b[i]);
    }

    /*printf("are_pbf = %lf\n", re_pbf_sum / b);
    printf("are_cbf = %lf\n", re_cbf_sum / b);*/

    printf("\n****************************************************\n");
    return 0;
}
