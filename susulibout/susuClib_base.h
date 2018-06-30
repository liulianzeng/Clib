#ifndef __BASE_DEFINE_H
#define __BASE_DEFINE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>



//#define DEBUG_BASE_MEM
//#define printf
void printdim(char *header,void *dim,int len);

#define mylocknoinit do{myclock(&lock);}while(0)
#define mylock static int lock;myclock(&lock)
#define myunlock do{mycunlock(&lock);}while(0)

extern void myclock(int *lock);
extern void mycunlock(int *lock);
extern void myasmlock(int *lock);
extern void myasmunlock(int *lock);


extern int GetStrSpaceBefore(char *src,int num,char *dst);

extern char * IntToString(int num);
extern int StringToInt(char *strSTI);
extern int c2i(char ch);
extern int hex2dec(char *hex);
extern int u8dimtoint1(unsigned char littleend,unsigned char * dimstart,unsigned char len);

#define Calloc_input calloc
#define Malloc_input malloc
#define Free_input free
#define DEBINFO 2,__FILE__,__LINE__

extern void * mycalloc (size_t count,size_t size);
extern void * mymalloc (size_t size);
extern void myfree (void *ptr);

#define MYCALLOC_IN(a,b) mycallocwithinfo(a,b,DEBINFO)
#define MYMALLOC_IN(a) mymallocwithinfo(a,DEBINFO)
#define MYFREE_IN(a) myfreewithinfo(a,DEBINFO)
extern void * mycallocwithinfo (size_t count,size_t size,int gs,...);
extern void * mymallocwithinfo (size_t size,int gs,...);
extern void myfreewithinfo (void *ptr,int gs,...);
extern void myfreeall(void);

extern int StrToInt(char *str1);
extern char * IntToStr(int value,char *n,char *str);
extern char * BcdToStr(int value,unsigned char n,char *str);//value 代表具体数值  n代表需产生多少位字符
extern char * TelIntToTelStr(long long value,char *str);
extern long long TelStrToTelInt(char *str2,char *str);
extern long long BCDToInt(unsigned char *str3,unsigned char num);
extern unsigned char ByteToBcd(unsigned char d);
extern unsigned char *IntToBcd(long long value,unsigned char n,unsigned char *len,char *str);
extern unsigned char *SearchKey(unsigned char *source,unsigned char len,unsigned char *dest);
extern int hexstrtoarrayu8(char *hexstr,void *array,int len);
extern void *memmemcustom(void *start, unsigned int s_len, void *find, unsigned int f_len);

#endif

