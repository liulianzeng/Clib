//作者 功能简介 库名称
//作者 susullz 库名称 ODLP
#ifndef __PROTOCOL_ODLP_H
#define __PROTOCOL_ODLP_H
//头文件的包含
#include <string.h>
#include "../lianbiaoquqelib/susuClib_quqe.h"
//#include "susuClib_quqe.h"
/*****************************define定义************************************/
#define ODLP_Finding_Header     0x00
#define ODLP_Finding_Length     0x01
#define ODLP_Finding_Pack       0x02
#define ODLP_Data_Finded        0x03

#define Not_enough_mem          0x81
#define Not_reach_Deadlength    0x82
#define Not_Init_header         0x83
#define SUBLENSTART_error       0x84


#define QUQE_T SeqQueue_t               //队列类型

/*****************************需要引入的函数********************************/
//需要输入的函数和变量--均需要在工程其他的地方实现或者赋初值  均采用extern的方式引入
//--内存申请释放引入
extern void * mycalloc (size_t count,size_t size);
extern void * mymalloc (size_t size);
extern void myfree (void *ptr);
//--base类函数
extern int u8dimtoint1(unsigned char littleend,unsigned char * dimstart,unsigned char len);
extern void *memmemcustom(void *start, unsigned int s_len, void *find, unsigned int f_len);
extern void printdim(char *header,void *dim,int len);
//--队列操作引入
extern SeqQueue_t *SeqQueue_Init(int maxlen);//初始化队列
extern int SeqQueue_In(SeqQueue_t *q,DATA_t data);//入队
extern int SeqQueue_ReverseIn(SeqQueue_t *q,DATA_t data);//反向入队
extern DATA_t *SeqQueue_Out(SeqQueue_t *q);//出队
extern int SeqQueue_Len(SeqQueue_t *q);//返回队列长度

extern int DEADLENGTH;// (2+12+1+2+1)         //最短包长
extern int SUBLENSTART;// (2+12+1)            //变长长度的起始位
extern int SUBLENLEN;// (2)                   //变长长度的字节数,默认为大端模式,若是小端模式,请使用(变长长度的字节数|0x80)
extern int MAXPACKLENGTH;// 300               //指定最长的协议长度  队列的默认长度将使用 本长度的 2倍数值  在不提前初始化的时候

/*  引入函数的默认写法   请写到自己的C函数中
void * mycalloc (size_t count,size_t size)
{
 return calloc(count,size);
}
void * mymalloc (size_t size)
{
 return malloc(size);
}
void myfree (void *ptr)
{
 free(ptr);
}
 //--队列操作
SeqQueue_t *SeqQueue_Init(int maxlen)//初始化队列
{
 return SeqQueue_tInit(maxlen);
}
int SeqQueue_In(SeqQueue_t *q,DATA_t data)//入队
{
 return SeqQueue_tIn(q,data);
}
int SeqQueue_ReverseIn(SeqQueue_t *q,DATA_t data)//反向入队
{
 return SeqQueue_tReverseIn(q,data);
}
DATA_t *SeqQueue_Out(SeqQueue_t *q)//出队
{
 return SeqQueue_tOut(q);
}
int SeqQueue_Len(SeqQueue_t *q)//返回队列长度
{
 return SeqQueue_tLen(q);
}
 int DEADLENGTH=(2+12+1+2+1);         //最短包长
 int SUBLENSTART=(2+12+1);            //变长长度的起始位
 int SUBLENLEN=(2);                   //变长长度的字节数,默认为大端模式,若是小端模式,请使用(变长长度的字节数|0x80)
 int MAXPACKLENGTH=300;               //指定最长的协议长度  队列的默认长度将使用 本长度的 2倍数值  在不提前初始化的时候

 */
/*****************************需要输出的函数********************************/
//需要输出的函数  变量的输出必采用extern的方式

int Setsize_Dynamic_Length_Protocol(unsigned char *__HEADER,int __HEADER_size);
extern int Input_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char data);
extern int Output_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char ** outp,int *len);



/*//使用示例
 
 */


#endif


