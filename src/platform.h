#ifndef TT_PLATFORM_H
#define TT_PLATFORM_H

#if defined(unix) || defined(__unix) || defined(__unix__)
# define TT_UNIX 1

# ifdef __linux__
#  define TT_LINUX 1
# endif
#elif defined(_WIN32)
# define TT_WINDOWS 1
#endif

int tt_time();
void tt_sleep(int ms);

#endif
