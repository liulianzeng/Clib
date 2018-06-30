#ifndef __PROTOCOL_ODLP_H
#define __PROTOCOL_ODLP_H
#include <string.h>
#include "../lianbiaoquqelib/susuClib_quqe.h"
//#include "susuClib_quqe.h"
//#include "kagoo_define.h"
extern int u8dimtoint1(unsigned char littleend,unsigned char * dimstart,unsigned char len);
extern void *memmemcustom(void *start, unsigned int s_len, void *find, unsigned int f_len);
extern void printdim(char *header,void *dim,int len);
#define SeqQueue_Init SeqQueue_tInit//队列初始化
#define SeqQueue_In SeqQueue_tIn//入队
#define SeqQueue_ReverseIn SeqQueue_tReverseIn
#define SeqQueue_Out SeqQueue_tOut//出队
#define SeqQueue_Len SeqQueue_tLen//队列长度测算
#define QUQE_T SeqQueue_t           //队列类型
#define DEADLENGTH (2+12+1+2+1)     //最短包长
#define SUBLENSTART (2+12+1)        //变长长度的起始位
#define SUBLENLEN (2)               //变长长度的字节数,默认为大端模式,若是小端模式,请使用(变长长度的字节数|0x80)
#define MAXPACKLENGTH 300           //指定最长的协议长度  队列的长度将使用 本长度的 2倍数值
//unsigned char HEADER[]={0xaa,0xa5};      //协议头部
extern void * mycalloc (size_t count,size_t size);
extern void * mymalloc (size_t size);
extern void myfree (void *ptr);


#define ODLP_Finding_Header 0x00
#define ODLP_Finding_Length 0x01
#define ODLP_Finding_Pack   0x02
#define ODLP_Data_Finded    0x03

#define Not_enough_mem      0x81
#define Not_reach_Deadlength 0x82
#define Not_Init_header     0x83
#define SUBLENSTART_error   0x84
int Setsize_Dynamic_Length_Protocol(unsigned char *__HEADER,int __HEADER_size);
extern int Input_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char data);
extern int Output_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char ** outp,int *len);


#endif


