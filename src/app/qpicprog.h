#ifndef QPICPROG_H
#define QPICPROG_H

#include <QObject>

class QPicProg : public QObject
{
protected:
    quint16 m_Port;
public:
    QPicProg();
	QPicProg(quint16 p_Port);
    bool Initport();
    void Data_Set_PGM();
    void Data_Clr_PGM();
    void Data_Clr_All();
    void ToggleClock();
	void Data_SetData(quint8 p_Data);
    quint8 Data_GetData();
};

#endif // QPICPROG_H
