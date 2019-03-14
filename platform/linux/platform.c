#include <platform.h>
#include <time.h>

void platform_init(void) {
}

uint32_t platform_ticks(void) {
	return (uint32_t)clock();
}

void platform_delay(uint32_t ms) {
	struct timespec req;
	req.tv_sec = (ms / 1000);
	req.tv_nsec = ((ms % 1000) * 1000);
	nanosleep(&req, NULL);
}

void platform_wait(void) {
	// TODO: sigwait
}
