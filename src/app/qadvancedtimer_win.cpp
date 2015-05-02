#include "qadvancedtimer.h"

#include <windows.h>

quint32 QAdvancedTimer::m_USec; // INFO: First time doing static class member, so I need to doublecheck the implementation.

//quint32 usec = 0;
quint32 cycles; // TODO: due to lack of gnu asm syntax I use temp var. Please fix me as it should be.

void QAdvancedTimer::setup()
{
    SetProcessAffinityMask(GetCurrentProcess(), 1);
    OSVERSIONINFO l_Ver;
    l_Ver.dwOSVersionInfoSize=sizeof(l_Ver);
    GetVersionEx(&l_Ver);
    if (l_Ver.dwPlatformId==VER_PLATFORM_WIN32_NT)
    {
        HKEY l_RegistryKey;
        DWORD l_Type, l_MHz, l_MHzSize = 4;
        RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &l_RegistryKey);
        RegQueryValueEx(l_RegistryKey, L"~MHZ", NULL, &l_Type, (LPBYTE)&l_MHz, &l_MHzSize);
        RegCloseKey(l_RegistryKey);

        QAdvancedTimer::m_USec = l_MHz;

    } else {
    }
}

void QAdvancedTimer::waitm(quint32 m_Interval)
{
    Sleep(m_Interval);
}

void QAdvancedTimer::waitu(quint32 m_Interval)
{
    if (QAdvancedTimer::m_USec == 0)
        QAdvancedTimer::setup();

    waitn(m_Interval * 1000);
}

void QAdvancedTimer::waitn(quint32 m_Interval)
{
    if (QAdvancedTimer::m_USec == 0)
        QAdvancedTimer::setup();

    cycles = QAdvancedTimer::m_USec * m_Interval / 1000;

#if defined(__x86_64__)
    __asm__ __volatile__ (
        "   push %rax\n"
        "   push %rbx\n"
        "   push %rcx\n"
        "   push %rdx\n"
        "   push %rsi\n"
        "   RDTSC\n"
        "   mov %edx, %esi\n"
        "   addl $cycles, %eax\n"
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
