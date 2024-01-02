#ifndef __TESTS_DECLARED
#define __TESTS_DECLARED

#include <ringbuffer.h>

#ifndef errno_t
typedef int errno_t;
#endif

extern errno_t test_single_shot (void);
extern errno_t test_synchronized (void);
extern errno_t test_slow_write (void);
extern errno_t test_slow_read (void);

#endif // __TESTS_DECLARED
