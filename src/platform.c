#include "platform.h"

#include <time.h>

#if defined(TT_UNIX)
# if _POSIX_C_SOURCE < 199309L
#  include <sys/select.h>
#  include <sys/time.h>
# endif
#elif defined(TT_WINDOWS)
# include <windows.h>
#endif

int tt_time(){
	return time(NULL);
}

void tt_sleep(int ms) {
#if defined(TT_UNIX)
# if _POSIX_C_SOURCE < 199309L
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
# else
	struct timespec ts;

	ts.tv_sec = (int)(ms / 1000);
	ts.tv_nsec = (ms % 1000) * 1000000;

	nanosleep(&ts, NULL);
# endif
#elif defined(TT_WINDOWS)
	Sleep(ms);
#endif
}
