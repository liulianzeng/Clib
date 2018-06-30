#ifndef __QUQE_DEFINE_H
#define __QUQE_DEFINE_H
//#include "mico.h"
////////////////////////////////以下是由数组实现的队列和栈/////////////////////////
#include <stdlib.h>
/*
#define quqemalloc malloc
#define quqecalloc calloc
#define quqefree free

#include "susuClib_base.h"
*/
extern void * mycalloc (size_t count,size_t size);
extern void * mymalloc (size_t size);
extern void myfree (void *ptr);
#define quqemalloc mymalloc
#define quqecalloc mycalloc
#define quqefree myfree

//#define printf(...)// printf()
#include <stdio.h>
//typedef int DATA_t;

typedef struct
{
    short cell;
    void * ptr;
}DATA_t;

typedef struct  
{  
    DATA_t *data;//队列数组
    int head;//队头   
    int tail;//队尾   
    int length;//队长
    int maxlength;
}SeqQueue_t;  

extern SeqQueue_t *SeqQueue_tInit(int maxlen);//初始化队列
extern void SeqQueue_tFree(SeqQueue_t *q);//释放队列 
extern int SeqQueue_tIsEmpty(SeqQueue_t *q);//队列是否为空，空为0  
extern int SeqQueue_tIsFull(SeqQueue_t *q);//队列是否为已满，满为1 
extern int SeqQueue_tLen(SeqQueue_t *q);//返回队列长度   
extern int SeqQueue_tIn(SeqQueue_t *q,DATA_t data);//入队 
extern DATA_t *SeqQueue_tOut(SeqQueue_t *q);//出队 
extern DATA_t *SeqQueue_tPeek(SeqQueue_t *q);//获取队头元素
extern DATA_t *SeqQueue_tTail(SeqQueue_t *q);//获取队尾元素
extern DATA_t *SeqQueue_tPos(SeqQueue_t *q,int posn);//获取指定位置的元素
extern int SeqQueue_tReverseIn(SeqQueue_t *q,DATA_t data);//入队//反向入队
extern DATA_t *SeqQueue_tReverseOut(SeqQueue_t *q);//出队//反向出队
/*//使用示例
#include<stdio.h>
#include<stdlib.h>
typedef struct{
    char key[15];
    char name[15];
}DATA;
#include"SeqQueue.h"

int main()
{
    DATA *data1,data;
    SeqQueue *q;
    q=SeqQueueInit();
    printf("input key name:");//一次入队
    scanf("%s%s",&data.key,&data.name);
    SeqQueueIn(q,data);

    data1=SeqQueueOut(q);//一次出队
    printf("(%s,%s)",data1->key,data1->name);
    SeqQueueFree(q);
    getch();
    return 0;
}
 */

#endif
