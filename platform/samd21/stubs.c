#include <board.h>
#include <platform/rtc.h>

#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#undef errno
extern int errno;

extern int _sstack;
extern int _end;

void *_sbrk(int nbytes) {
	static void *heap = NULL;
	void *prev_heap;

	if(heap == NULL) {
		heap = (void *)&_end;
	}
	prev_heap = heap;
	heap += nbytes;
	return prev_heap;
}

int _write(int __fd, const void *__buf, size_t __nbyte) {
	if(__fd == STDOUT_FILENO || __fd == STDERR_FILENO) {
		console_write((uint8_t *)__buf, (size_t)__nbyte);
		return __nbyte;
	}else{
		errno = EBADF;
		return -1;
	}
}

/*
int _isatty(int file) {
	if(file <= STDERR_FILENO) {
		return 1;
	}else{
		return 0;
	}
}
*/

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
