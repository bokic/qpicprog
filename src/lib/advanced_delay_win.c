#include "advanced_delay.h"
#include "export.h"

#ifdef WIN32
#include <windows.h>
#endif

//#include <stdbool.h>
#include <stdint.h>

uint32_t usec = 0;


MY_EXPORT void adv_delay_setup()
{
    OSVERSIONINFOW l_version;

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    if (usec == 0)
    {
        memset(&l_version, 0, sizeof(l_version));
        l_version.dwOSVersionInfoSize=sizeof(l_version);
        GetVersionEx(&l_version);

        if (l_version.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
            HKEY l_RegistryKey;
            DWORD l_Type, l_MHz, l_MHzSize = 4;
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &l_RegistryKey);
            RegQueryValueEx(l_RegistryKey, L"~MHZ", NULL, &l_Type, (LPBYTE)&l_MHz, &l_MHzSize);
            RegCloseKey(l_RegistryKey);

            usec = l_MHz;
        }
    }
}

MY_EXPORT void adv_delaym(uint32_t interval)
{
    Sleep(interval);
}

MY_EXPORT void adv_delayu(uint32_t interval)
{
    if (usec == 0)
    {
        adv_delay_setup();
    }

    adv_delayn(interval * 1000);
}

#ifdef __GNUC__
//#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"

inline uint64_t RDTSC()
{
    __asm__("RDTSC\n");
}

#pragma GCC diagnostic warning "-Wreturn-type"
//#pragma GCC diagnostic pop
#elif defined _MSC_VER
#define RDTSC() (__rdtsc())
#else
#error Unsupported compiler.
#endif

MY_EXPORT void adv_delayn(uint32_t interval)
{
    uint64_t current, cycles, start;

    start = RDTSC();

    if (usec == 0)
    {
        adv_delay_setup();
    }

    cycles = (usec * interval) / 1000;

    while(1)
    {
        current = RDTSC();
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
