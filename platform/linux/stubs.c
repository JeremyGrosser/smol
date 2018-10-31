#include <board.h>
#include <platform/rtc.h>

#include <errno.h>
#include <unistd.h>
#include <time.h>

int _gettimeofday(struct timeval *tv, void *tz) {
	struct tm *now;
	now = rtc_read();
	if(now == NULL) {
		errno = EINVAL;
		return 1;
	}
	tv->tv_usec = 0;
	tv->tv_sec = mktime(rtc_read());
	if(tv->tv_sec == -1) {
		return 1;
	}
	return 0;
}
