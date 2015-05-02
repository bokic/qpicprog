#ifndef QADVANCEDTIMER_H
#define QADVANCEDTIMER_H

#include "q16f87xpicprog.h"
#include "qds33picprog.h"
#include <QObject>

class QAdvancedTimer: public QObject
{
	friend class Q16F87xPicProg;
	friend class Qds33PicProg;
public:
    static void setup();
	static void waitm(quint32 m_Interval);
	static void waitu(quint32 m_Interval);
	static void waitn(quint32 m_Interval);
private:
	static quint32 m_USec;
};

#endif // QADVANCEDTIMER_H
