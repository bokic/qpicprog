#include "qadvancedtimer.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QFile>
#include <QMap>
#include <sched.h>
#include <sys/param.h>

quint32 QAdvancedTimer::m_USec; // INFO: First time doing static class member, so I need to doublecheck the implementation.

//quint32 usec = 0;
quint32 cycles; // TODO: due to lack of gnu asm syntax I use temp var. Please fix me as it should be.

void QAdvancedTimer::setup()
{
    // Set this program to run only on first code(if more than one)

    QFile l_File("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");

    if (l_File.open(QIODevice::ReadOnly) == true)
    {
        QByteArray l_ba = l_File.readAll();
        l_File.close();

        l_ba = l_ba.split('\n').at(0); // Get first line.
        m_USec = l_ba.toInt();
    }
    else
    {
        bool ok;
        QAdvancedTimer::m_USec = QInputDialog::getInt(NULL, tr("Please enter CPU frequency"), tr("CPU frequency count\'t be aquired, please enter the CPU frequency(MHz)."), 0, 1000, 10000, 100, &ok);

        if (ok == false)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("The CPU frequency was not set, so 2GHz CPU speed will be set."));
            QAdvancedTimer::m_USec = 2000;
        }
    }
}

void QAdvancedTimer::waitm(quint32 m_Interval)
{
    usleep(m_Interval * 1000);
}

void QAdvancedTimer::waitu(quint32 m_Interval)
{
    usleep(m_Interval);
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
