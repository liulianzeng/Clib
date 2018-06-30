#include "susuunixrtc.h"	  
int hour=8,min=0,sec=0,year=2009,mon=11,day=13;
static struct tm nowc,*now;
char DataTimestr[20]="";
struct tm timetmpr;
extern void RTC_WaitForLastTask(void);
extern long  RTC_GetCounter(void);
extern void RTC_SetCounter(long CounterValue);
void Time_Adjust(void);
unsigned long Time_Regulate(void);
extern struct tm Time_ConvUnixToCalendar(time_t t);
extern time_t Time_ConvCalendarToUnix(struct tm t);
extern time_t Time_GetUnixTime(void);
extern struct tm Time_GetCalendarTime(void);
extern void Time_SetUnixTime(time_t);
extern void Time_SetCalendarTime(struct tm t);
void Time_Set(long t);
char *DataTimeToDTStr(struct tm timetmp);
struct tm DTStrToDataTime(const char *Strtmp);
unsigned long get_fattime (void);
//将指定的时间字符串转换为tm格式的值
struct tm DTStrToDataTime(const char *Strtmp);

void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(Time_Regulate());
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
 
/*******************************************************************************
* Function Name  : Time_Regulate
* Description    : Returns the time entered by user, using Hyperterminal.
* Input          : None
* Output         : None
* Return         : Current time RTC counter value
*******************************************************************************/
unsigned long Time_Regulate(void)
{
  /* Return the value to store in RTC counter register */
 // return((hour*3600 + min*60 + sec));
  	nowc.tm_year = 114;
	nowc.tm_mon = 1;        //11月
	nowc.tm_mday = 1;
	nowc.tm_hour = 1;
	nowc.tm_min = 0;
	nowc.tm_sec = 0;
	now=&nowc;
	return((long)mktime(now));
}
 /*******************************************************************************
* 本文件实现基于RTC的日期功能，提供年月日的读写。（基于ANSI-C的time.h）
* 
* RTC中保存的时间格式，是UNIX时间戳格式的。即一个32bit的time_t变量（实为long）
*
* ANSI-C的标准库中，提供了两种表示时间的数据  型：
* time_t:   	UNIX时间戳（从1970-1-1起到某时间经过的秒数）
* 	typedef unsigned int time_t;
* 
* struct tm:	Calendar格式（年月日形式）
*   tm结构如下：
*   struct tm {
*   	int tm_sec;   // 秒 seconds after the minute, 0 to 60
*   					 (0 - 60 allows for the occasional leap second)
*   	int tm_min;   // 分 minutes after the hour, 0 to 59
*		int tm_hour;  // 时 hours since midnight, 0 to 23
*		int tm_mday;  // 日 day of the month, 1 to 31
*		int tm_mon;   // 月 months since January, 0 to 11
*		int tm_year;  // 年 years since 1900
*		int tm_wday;  // 星期 days since Sunday, 0 to 6
*		int tm_yday;  // 从元旦起的天数 days since January 1, 0 to 365
* 		int tm_isdst; // 夏令时？？Daylight Savings Time flag
* 		...
* 	}
* 	其中wday，yday可以自动产生，软件直接读取
* 	mon的取值为0-11
*	***注意***：
*	tm_year:在time.h库中定义为1900年起的年份，即2008年应表示为2008-1900=108
* 	这种表示方法对用户来说不是十分友好，与现实有较大差异。
* 	所以在本文件中，屏蔽了这种差异。
* 	即外部调用本文件的函数时，tm结构体类型的日期，tm_year即为2008
* 	注意：若要调用系统库time.c中的函数，需要自行将tm_year-=1900
* 
* 成员函数说明：
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	输入一个Unix时间戳（time_t），返回Calendar格式日期
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	输入一个Calendar格式日期，返回Unix时间戳（time_t）
* time_t Time_GetUnixTime(void);
* 	从RTC取当前时间的Unix时间戳值
* struct tm Time_GetCalendarTime(void);
* 	从RTC取当前时间的日历时间
* void Time_SetUnixTime(time_t);
* 	输入UNIX时间戳格式时间，设置为当前RTC时间
* void Time_SetCalendarTime(struct tm t);
* 	输入Calendar格式时间，设置为当前RTC时间
* 
* 外部调用实例：
* 定义一个Calendar格式的日期变量：
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12月
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* 获取当前日期时间：
* tm_now = Time_GetCalendarTime();
* 然后可以直接读tm_now.tm_wday获取星期数
* 
* 设置时间：
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* 计算两个时间的差
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt就是两个时间差的秒数
* dt_tm = mktime(dt);	//注意dt的年份匹配，ansi库中函数为相对年份，注意超限
* 另可以参考相关资料，调用ansi-c库的格式化输出等功能，ctime，strftime等
* 
*******************************************************************************/

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Description    : 转换UNIX时间戳为日历时间
* Input 		 : long t  当前时间的UNIX时间戳
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtime转换结果的tm_year是相对值，需要转成绝对值
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : 写入RTC时钟当前时间
* Input 		 : struct tm t
* Output		 : None
* Return		 : time_t
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //外部tm结构体存储的年份为2008格式
						//而time.h中定义的年份格式为1900年开始的年份
						//所以，在日期转换时要考虑到这个因素。
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : 从RTC取当前时间的Unix时间戳值
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime()
* Description    : 从RTC取当前时间的日历时间（struct tm）
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t);
	t_tm.tm_mon=t_tm.tm_mon+1;
	return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : 将给定的Unix时间戳写入RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((long)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	t.tm_mon=t.tm_mon-1;
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}
/*******************************************************************************
* Function Name  : get_fattime()
* Description    : 特殊模块获取时间
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
unsigned long get_fattime (void)
{
	struct tm rtc;		  

	rtc=Time_GetCalendarTime();

	return	  ((unsigned long)(rtc.tm_year - 1980) << 25)
			| ((unsigned long)rtc.tm_mon << 21)
			| ((unsigned long)rtc.tm_mday << 16)
			| ((unsigned long)rtc.tm_hour << 11)
			| ((unsigned long)rtc.tm_min << 5)
			| ((unsigned long)rtc.tm_sec >> 1);
}
char *DataTimeToDTStr(struct tm timetmp)
{
	DataTimestr[0]=timetmp.tm_year/1000%10+'0';
	DataTimestr[1]=timetmp.tm_year/100%10+'0';
	DataTimestr[2]=timetmp.tm_year/10%10+'0';
	DataTimestr[3]=timetmp.tm_year%10+'0';
	DataTimestr[4]='-';
	DataTimestr[5]=timetmp.tm_mon/10%10+'0';
	DataTimestr[6]=timetmp.tm_mon%10+'0';
	DataTimestr[7]='-';
	DataTimestr[8]=timetmp.tm_mday/10%10+'0';
	DataTimestr[9]=timetmp.tm_mday%10+'0';
	DataTimestr[10]=' ';
	DataTimestr[11]=timetmp.tm_hour/10%10+'0';
	DataTimestr[12]=timetmp.tm_hour%10+'0';
	DataTimestr[13]=':';
	DataTimestr[14]=timetmp.tm_min/10%10+'0';
	DataTimestr[15]=timetmp.tm_min%10+'0';
	DataTimestr[16]=':';
	DataTimestr[17]=timetmp.tm_sec/10%10+'0';
	DataTimestr[18]=timetmp.tm_sec%10+'0';
	DataTimestr[19]=0;
	return DataTimestr;
}
struct tm DTStrToDataTime(const char *Strtmp)
{
	timetmpr.tm_year=(Strtmp[0]-'0')*1000+(Strtmp[1]-'0')*100+(Strtmp[2]-'0')*10+(Strtmp[3]-'0');
	timetmpr.tm_mon=(Strtmp[5]-'0')*10+(Strtmp[6]-'0');
	timetmpr.tm_mday=(Strtmp[8]-'0')*10+(Strtmp[9]-'0');
	timetmpr.tm_hour=(Strtmp[11]-'0')*10+(Strtmp[12]-'0');
	timetmpr.tm_min=(Strtmp[14]-'0')*10+(Strtmp[15]-'0');
	timetmpr.tm_sec=(Strtmp[17]-'0')*10+(Strtmp[18]-'0');
	return timetmpr;
}







