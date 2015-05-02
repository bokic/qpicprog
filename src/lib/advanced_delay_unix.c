#include "advanced_delay.h"
#include "export.h"

#include <sys/param.h>
#include <sched.h>
#include <unistd.h>
#include <sched.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>

uint32_t usec = 0;


MY_EXPORT void adv_delay_setup()
{
    // Set this program to run only on first code(if more than one)
    //sched_setaffinity(getpid(), );

    int handle = open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", O_RDONLY);

    if (handle)
    {
        char buf[20];
        memset(buf, 0, sizeof(buf));
        if (read(handle, buf, 20) > 0)
        {
            usec = atoi(buf) / 1000;
        }
        close(handle);
    }
}

MY_EXPORT void adv_delaym(uint32_t interval)
{
    usleep(interval * 1000);
}

MY_EXPORT void adv_delayu(uint32_t interval)
{
    usleep(interval);
}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

inline uint64_t RDTSC()
{
    __asm__("RDTSC\n");
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

MY_EXPORT void adv_delayn(uint32_t interval)
{
    uint64_t start = RDTSC();

    if (usec == 0)
    {
        adv_delay_setup();
    }

    uint64_t cycles = (usec * interval) / 1000;

    while(true)
    {
        uint64_t current = RDTSC();
        if ((current <= 0xffffffff)&&(start > 0xffffffff))
        {
            if (~(start - current) >= cycles)
            {
                break;
            }
        }
        else
        {
            if (current - start >= cycles)
            {
                break;
            }
        }
    }
}
