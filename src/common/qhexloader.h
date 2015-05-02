#ifndef QHEXLOADER_H
#define QHEXLOADER_H

#include <QByteArray>
#include <QObject>

struct QHexRow{
    quint8 m_Type;
    quint16 m_Address;
    QByteArray m_Data;
};

class QHexLoader : public QObject
{
public:
    QHexLoader(const QString &p_Filename = "", QObject *parent = NULL);
    bool load(const QString &p_Filename);

    QList<QHexRow> data();

private:

    QList<QHexRow> m_Rows;
};

#endif // QHEXLOADER_H
