//作者 功能简介 库名称
//作者 susullz 库名称 base
#ifndef __QUQE_DEFINE_H
#define __QUQE_DEFINE_H
//头文件的包含
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*****************************define定义************************************/




typedef struct
{
    short cell;
    void * ptr;
    void * previous;
    void * point;
    void * next;
}DATA_t;

typedef struct
{
    DATA_t data;//队列数据
    DATA_t *head;//队头
    DATA_t *tail;//队尾
    int length;//队长
    int maxlength;
}SeqQueue_t;

/*****************************需要引入的函数********************************/
//需要输入的函数和变量--均需要在工程其他的地方实现或者赋初值  均采用extern的方式引入

//内存管理
#define BASE_DEBINFO 2,__FILE__,__LINE__
#define mycalloc(a,b) mycallocwithinfo(a,b,BASE_DEBINFO)
#define mymalloc(a) mymallocwithinfo(a,BASE_DEBINFO)
#define myfree(a) myfreewithinfo(a,BASE_DEBINFO)
#define myfreeall() myfreeallwithinfo(BASE_DEBINFO)
//内存管理内部的调用,也可以自己编写调用,目前指定的参数为2个扩展参数,第一个扩展为char * ,第二个扩展为int.
extern void * mycallocwithinfo (size_t count,size_t size,int gs,...);
extern void * mymallocwithinfo (size_t size,int gs,...);
extern void myfreewithinfo (void *ptr,int gs,...);
extern void myfreeallwithinfo(int gs,...);

extern void mylocknoinit(void);
extern void mylock(void);
extern void myunlock(void);

/*  引入函数的默认写法   请写到自己的C函数中
 //内存申请入口
void * mycalloc (size_t count,size_t size)
{
 return calloc(count,size);
}
void * mymalloc (size_t size)
{
 malloc(size);
}
void myfree (void *ptr)
{
 free(ptr);
}
void myfreeall (void)
{
 ;
}
 //--引入临界区的入口
void mylocknoinit(void)
{
 myclock(&lock);
}
void mylock(void)
{
 myclock(&lock);
}
void myunlock(void)
{
 mycunlock(&lock);
}
 */
/*****************************需要输出的函数********************************/
//需要输出的函数  变量的输出必采用extern的方式
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
extern int SeqQueue_tHeadPosnIn(SeqQueue_t *q,DATA_t data,int posn);//队首开始第n个插入队
extern DATA_t *SeqQueue_tHeadPosnOut(SeqQueue_t *q,int posn);////队首开始第n个插出队
extern int SeqQueue_tTailPosnIn(SeqQueue_t *q,DATA_t data,int posn);////队尾开始第n个插入队
extern DATA_t *SeqQueue_tTailPosnOut(SeqQueue_t *q,int posn);////队尾开始第n个插出队


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
