//作者 功能简介 库名称  
//作者 susullz 库名称 base
#ifndef __BASE_DEFINE_H
#define __BASE_DEFINE_H
//头文件的包含
#include <string.h>
#include <stdio.h>
/*****************************define定义************************************/
//#define DEBUG_BASE_MEM
//#define printf

//#define BASE_DEBINFO 2,__FILE__,__LINE__
//#define MYCALLOC_IN(a,b) mycallocwithinfo(a,b,BASE_DEBINFO)
//#define MYMALLOC_IN(a) mymallocwithinfo(a,BASE_DEBINFO)
//#define MYFREE_IN(a) myfreewithinfo(a,BASE_DEBINFO)
//#define MYFREEALL_IN() myfreeallwithinfo(BASE_DEBINFO)

/*
#define BASE_DEBINFO 2,__FILE__,__LINE__
#define mycalloc(a,b) mycallocwithinfo(a,b,BASE_DEBINFO)
#define mymalloc(a) mymallocwithinfo(a,BASE_DEBINFO)
#define myfree(a) myfreewithinfo(a,BASE_DEBINFO)
#define myfreeall() myfreeallwithinfo(BASE_DEBINFO)
*/

//#define mylocknoinit() do{myclock(&lock);}while(0)
//#define mylock() static int lock;myclock(&lock)
//#define myunlock() do{mycunlock(&lock);}while(0)

//#define mylocknoinit do{myasmlock(&lock);}while(0)
//#define mylock static int lock;myasmlock(&lock)
//#define myunlock do{myasmunlock(&lock);}while(0)

extern int DEBUG_BASE_MEM;
/*****************************需要引入的函数********************************/
//需要输入的函数和变量--均需要在工程其他的地方实现或者赋初值  均采用extern的方式引入
//--引入临界区的入口
extern void mylocknoinit(void);
extern void mylock(void);
extern void myunlock(void);
//--引入申请内存的入口
extern void * Calloc_input (size_t count,size_t size);
extern void * Malloc_input (size_t size);
extern void Free_input (void *ptr);
//--引入打印日志的入口
extern int print_base(char * info,...);
/*  引入函数的默认写法   请写到自己的C函数中
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
//--引入申请内存的入口
void * Calloc_input (size_t count,size_t size)
{
	return calloc(count,size);
}
void * Malloc_input (size_t size)
{
	return malloc(size);
}
void Free_input (void *ptr)
{
	free(ptr);
}
//--引入打印日志的入口
int print_base(char * info,...)
{
	int ret;
	va_list ap;

	va_start(ap, info);
	ret = vprintf(info, ap);
	va_end(ap);
 return ret;
}
*/
/*****************************需要输出的函数********************************/
//需要输出的函数  变量的输出必采用extern的方式
//超简单临界区锁的实现,不是哪里都能用的
extern void myclock(int *lock);
extern void mycunlock(int *lock);
extern void myasmlock(int *lock);
extern void myasmunlock(int *lock);
//从src开始获取指定的第num个的字符串,放置到dst中,并返回dst的字符串的长度
extern int GetStrSpaceBefore(char *src,int num,char *dst);
//数字间的互转  自己编写代码实现的
extern char * IntToString(int num);
extern int StringToInt(char *strSTI);
extern int c2i(char ch);
extern int hex2dec(char *hex);
//数字间的互转  库函数编写实现的
extern int StrToInt(char *str1);
extern char * IntToStr(int value,char *n,char *str);
extern char * BcdToStr(int value,unsigned char n,char *str);//value 代表具体数值  n代表需产生多少位字符
extern char * TelIntToTelStr(long long value,char *str);
extern long long TelStrToTelInt(char *str2,char *str);
extern long long BCDToInt(unsigned char *str3,unsigned char num);
extern unsigned char ByteToBcd(unsigned char d);
extern unsigned char *IntToBcd(long long value,unsigned char n,unsigned char *len,char *str);
//根据大小端格式将len个元素的u8的数组dimstart转换成整数返回
extern int u8dimtoint1(unsigned char littleend,unsigned char * dimstart,unsigned char len);
//内存管理
//extern void * mycalloc (size_t count,size_t size);
//extern void * mymalloc (size_t size);
//extern void myfree (void *ptr);
//extern void myfreeall (void);
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
//不使用strstr(长度为len)中在source中寻找2字节内的字符串dest的指针位置,该函数是早期用来寻找AT指令的AT所在位置的函数
extern unsigned char *SearchKey(unsigned char *source,unsigned char len,unsigned char *dest);
extern int hexstrtoarrayu8(char *hexstr,void *array,int len);
extern char * arrayu8tohexstr(void *array,int len,char *hexstr);
extern void *memmemcustom(void *start, unsigned int s_len, void *find, unsigned int f_len);//类似于strstr的数组查找子数组的函数
extern void printdim(char *header,void *dim,int len);//打印数组
#endif

