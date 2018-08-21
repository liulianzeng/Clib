//作者 功能简介 库名称
//作者 susullz 库名称 ODLP
#ifndef __PROTOCOL_ODLP_H
#define __PROTOCOL_ODLP_H
//头文件的包含
#include <string.h>
#include "../DimMinquqelib/susuClib_quqe.h"
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
//--base类函数
extern int u8dimtoint1(unsigned char littleend,unsigned char * dimstart,unsigned char len);
extern void *memmemcustom(void *start, unsigned int s_len, void *find, unsigned int f_len);
extern void printdim(char *header,void *dim,int len);
//--队列操作引入
extern SeqQueue_t *SeqQueue_Init(int maxlen,unsigned char unitlen);//初始化队列
extern int SeqQueue_In(SeqQueue_t *q,unsigned char * data);//入队
extern int SeqQueue_ReverseIn(SeqQueue_t *q,unsigned char * data);//反向入队
extern unsigned char *SeqQueue_Out(SeqQueue_t *q);//出队
extern int SeqQueue_Len(SeqQueue_t *q);//返回队列长度


extern int DEADLENGTH;// (2+12+1+2+1)         //最短包长
extern int SUBLENSTART;// (2+12+1)            //变长长度的起始位
extern int SUBLENLEN;// (2)                   //变长长度的字节数,默认为大端模式,若是小端模式,请使用(变长长度的字节数|0x80)
extern int MAXPACKLENGTH;// 300               //指定最长的协议长度  队列的默认长度将使用 本长度的 2倍数值  在不提前初始化的时候
extern int FIXEDLEN;//0						  //当变长长度中包含固定长度的数据时,该数据用来做调整,该值是可变长度中包含的固定数据的长度
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
SeqQueue_t *SeqQueue_Init(int maxlen,unsigned char unitlen)//初始化队列
{
    return SeqQueue_tInit(maxlen,unitlen);
}
int SeqQueue_In(SeqQueue_t *q,unsigned char * data)//入队
{
    return SeqQueue_tIn(q,data);
}
int SeqQueue_ReverseIn(SeqQueue_t *q,unsigned char * data)//反向入队
{
    return SeqQueue_tReverseIn(q,data);
}
unsigned char *SeqQueue_Out(SeqQueue_t *q)//出队
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

int Setsize_Dynamic_Length_Protocol(unsigned char *__HEADER,int __HEADER_size);//旧版初始化   协议头初始化 参数 协议头数组 协议头数组长度
extern int Init_Dynamic_Protocol_Header(unsigned char *__HEADER,int __HEADER_size);//协议头初始化 参数 协议头数组 协议头数组长度
extern int Input_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char data);//输入协议数据  参数  接收数据的队列  接受的数据(字节)
extern int Output_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char ** outp,int *len);//输出协议函数 当返回值为ODLP_Data_Finded 0x03时表示已经获得完整的一包数据,若想要获取完整的一包数据,至少要调用四次本函数,参数 接收数据的队列 返回完整的一包数据的指针的指针,返回完整的一包数据的长度  返回的数据是使用mymalloc获得的内存  处理完成后必须使用myfree释放

/*//使用示例
 //初始化处理程序
	udpreceivedq=SeqQueue_tInit(MAXPACKLENGTHQ,sizeof(uint8_t));//队列初始化  队列的默认长度将使用 本长度的 2倍数值  在不提前初始化的时候  这里初始化可以避免使用2倍MAXPACKLENGTH长度 但2倍长度也很合适
	Init_Dynamic_Protocol_Header(HEADER,sizeof(HEADER));
//	Setsize_Dynamic_Length_Protocol(HEADER,sizeof(HEADER));//旧版初始化  协议头数组
 //接收数据的回调处理程序
 AppSocketReceiveProc(gsSTSysCtrol.FdUdp,data,len);
mylock();//尽量使用锁  防止数据并发执行
if(data)//如果获取的数据有数据
for(int i=0;i<len;i++)//将数据加入到队列中
{
		Input_Dynamic_Length_Protocol(udpreceivedq,data[i]);//将数据加入到队列中  其实就是一个入队列的操作 
}
myunlock();
 //接收数据的处理程序
 typedef struct
{
    short cell;
    void * ptr;
}DATA_t;
int len;
int ODLP_Stat=0;
unsigned char * procp;
unsigned char ** proc=&procp;
DATA_t recdat;
int TimesofODLP=0;

if(SeqQueue_Len(udpreceivedq))//若收到数据
for(int i=0;i<4;i++)
{
	ODLP_Stat=Output_Dynamic_Length_Protocol(udpreceivedq,proc,&len);//判断输出数据 至少要判断四次以上
	if(ODLP_Stat==ODLP_Data_Finded)//如果收到了完整的数据
	{
		recdat.cell=(uint16_t)len;//获取数据长度
		recdat.ptr=*proc;//获取数据本体
		myapp_log( "len is %d",len);//打印长度
		SeqQueue_In(udpprocq,(unsigned char *)&recdat);//将获得的数据包压入消息处理队列
		TimesofODLP=0;//获取的次数清空
	}
	else if(TimesofODLP++>10)//如果获取的次数超过了10次
	{
		TimesofODLP=0;//清空获取的数据次数
		for(i=0;i<SeqQueue_Len(udpreceivedq);i++)//清空接收 数据的队列    十次已经说明数据的错误
		{
			SeqQueue_Out(udpreceivedq);
		}
		myapp_log( "Output_Dynamic_Length_Protocol, ODLP_Stat is %d",(int)ODLP_Stat);
		myapp_log( "not value data in udpreceivedq ,empty it!!");
	}
}
 */


#endif


