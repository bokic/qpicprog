#include "advanced_delay.h"
#include "export.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <stdbool.h>
#include <stdint.h>

uint32_t usec = 0;
uint32_t cycles; // TODO: due to lack of gnu asm syntax I use temp var. Please fix me as it should be.


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

MY_EXPORT void adv_delayn(uint32_t interval)
{
    if (usec == 0)
    {
        adv_delay_setup();
    }

    cycles = usec * interval / 1000;

#if defined(__x86_64__)
    __asm__ __volatile__ (
        "   push %rax\n"
        "   push %rbx\n"
        "   push %rcx\n"
        "   push %rdx\n"
        "   push %rsi\n"
        "   RDTSC\n"
        "   mov %edx, %esi\n"
        "   addl $_cycles, %eax\n"
        "   jnc NoOverlap\n"
        "   inc %edx\n"
        "NoOverlap:\n"
        "   mov %eax, %ebx\n"
        "   mov %edx, %ecx\n"
        "Loop1:\n"
        "   RDTSC\n"
        "   cmp %edx, %esi\n"
        "   jb Loop3\n"
        "   cmp %ecx, %edx\n"
        "   jne Loop1\n"
        "Loop2:\n"
        "   cmp %ebx, %eax\n"
        "   jbe Loop1\n"

        "Loop3:\n"
        "   pop %rsi\n"
        "   pop %rdx\n"
        "   pop %rcx\n"
        "   pop %rbx\n"
        "   pop %rax\n"
    );
#else
    __asm__ __volatile__ (
        "   push %eax\n"
        "   push %ebx\n"
        "   push %ecx\n"
        "   push %edx\n"
        "   push %esi\n"

        "   RDTSC\n"
        "   mov %edx, %esi\n"
        "   addl $_cycles, %eax\n"
        "   jnc NoOverlap\n"
        "   inc %edx\n"
        "NoOverlap:\n"
        "   mov %eax, %ebx\n"
        "   mov %edx, %ecx\n"
        "Loop1:\n"
        "   RDTSC\n"
        "   cmp %edx, %esi\n"
        "   jb Loop3\n"
        "   cmp %ecx, %edx\n"
        "   jne Loop1\n"
        "Loop2:\n"
        "   cmp %ebx, %eax\n"
        "   jbe Loop1\n"

        "Loop3:\n"
        "   pop %esi\n"
        "   pop %edx\n"
        "   pop %ecx\n"
        "   pop %ebx\n"
        "   pop %eax\n"
    );
#endif
}
