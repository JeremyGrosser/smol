#ifndef RTC_H
#define RTC_H
#include <sys/time.h>

void rtc_setup(void);
struct tm *rtc_read(void);
void rtc_write(struct tm *now);

#endif
