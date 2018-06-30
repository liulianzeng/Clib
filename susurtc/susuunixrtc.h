#ifndef __RTC_H
#define __RTC_H

#include <time.h>
extern void RTC_WaitForLastTask(void);
extern long  RTC_GetCounter(void);
extern void RTC_SetCounter(long CounterValue);

extern time_t Time_GetUnixTime(void);
extern void Time_SetCalendarTime(struct tm t);
unsigned long get_fattime (void);
struct tm DTStrToDataTime(const char *Strtmp);

#endif 
