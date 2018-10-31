#ifndef RTC_H
#define RTC_H
#include <sys/time.h>

void rtc_init(void);
struct tm *rtc_read(void);
void rtc_write(struct tm *now);

#endif
