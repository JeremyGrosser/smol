#include <platform/rtc.h>
#include <linux/rtc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

static struct tm now;

void rtc_init() {
}

struct tm *rtc_read() {
	struct rtc_time rt;
	int fd;
	int err;

	fd = open("/dev/rtc", O_RDONLY);
	err = ioctl(fd, RTC_RD_TIME, &rt);
	if(err != 0) {
		perror("rtc_read failed: ");
		return NULL;
	}
	close(fd);

	now.tm_sec = rt.tm_sec;
	now.tm_min = rt.tm_min;
	now.tm_hour = rt.tm_hour;
	now.tm_mday = rt.tm_mday;
	now.tm_mon = rt.tm_mon;
	now.tm_year = rt.tm_year;

	return &now;
}

void rtc_write(struct tm *now) {
}
