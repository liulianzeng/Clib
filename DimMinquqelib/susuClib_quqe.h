//作者 功能简介 库名称
//作者 susullz 库名称 base
#ifndef __QUQE_DEFINE_H
#define __QUQE_DEFINE_H

////////////////////////////////以下是由最小数组short地址实现的队列和栈/////////////////////////
//头文件的包含
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*****************************define定义************************************/
#define u16 unsigned short
#define u8 unsigned char

typedef struct  
{  
    void *data;//队列数组
    u16 head;//队头   
    u16 tail;//队尾   
    u16 length;//队长
    u16 maxlength;//最大长度
		u8 unitlength;//队列单个元素的长度
}SeqQueue_t;  
/*****************************需要引入的函数********************************/
//需要输入的函数和变量--均需要在工程其他的地方实现或者赋初值  均采用extern的方式引入

//内存管理
/*#include <stdlib.h>

#define quqemalloc malloc
#define quqecalloc calloc
#define quqefree free

#include "susuClib_base.h"
*/
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
#define quqemalloc mymalloc
#define quqecalloc mycalloc
#define quqefree myfree
//锁
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
extern SeqQueue_t *SeqQueue_tInit(u16 maxlen,u8 unitlen);//初始化队列
extern void SeqQueue_tFree(SeqQueue_t *q);//释放队列 
extern u16 SeqQueue_tIsEmpty(SeqQueue_t *q);//队列是否为空，空为0  
extern u16 SeqQueue_tIsFull(SeqQueue_t *q);//队列是否为已满，满为1 
extern u16 SeqQueue_tLen(SeqQueue_t *q);//返回队列长度   
extern u16 SeqQueue_tIn(SeqQueue_t *q,u8 *data);//入队 
extern void *SeqQueue_tOut(SeqQueue_t *q);//出队 
extern void *SeqQueue_tPeek(SeqQueue_t *q);//获取队头元素
extern void *SeqQueue_tTail(SeqQueue_t *q);//获取队尾元素
extern void *SeqQueue_tPos(SeqQueue_t *q,u16 posn);//获取指定位置的元素
extern u16 SeqQueue_tReverseIn(SeqQueue_t *q,u8 *data);//入队//反向入队
extern void *SeqQueue_tReverseOut(SeqQueue_t *q);//出队//反向出队


#endif
