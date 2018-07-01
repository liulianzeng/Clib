//作者 功能简介 库名称
//作者 susullz 库名称 base
#ifndef __RTC_H
#define __RTC_H
//头文件的包含
#include <time.h>

/*****************************define定义************************************/




/*****************************需要引入的函数********************************/
//需要输入的函数和变量--均需要在工程其他的地方实现或者赋初值  均采用extern的方式引入
extern void RTC_WaitForLastTask(void);
extern long  RTC_GetCounter(void);
extern void RTC_SetCounter(long CounterValue);


/*  引入函数的默认写法   请写到自己的C函数中
 void RTC_WaitForLastTask(void)
 {
 
 }
 long  RTC_GetCounter(void)
 {
 return 0;
 }
 void RTC_SetCounter(long CounterValue)
 {
 
 }
 
 */
/*****************************需要输出的函数********************************/
//需要输出的函数  变量的输出必采用extern的方式
extern time_t Time_GetUnixTime(void);
extern void Time_SetCalendarTime(struct tm t);
extern unsigned long get_fattime (void);
struct tm DTStrToDataTime(const char *Strtmp);



/*//使用示例
 
 */



#endif 
