#include <QCoreApplication>
#include <QStringList>
#include <QMessageBox>

#include <advanced_delay.h>
#include <picprog_16f87x.h>
#include <picprog_ds.h>
#include <picprog.h>

#include <qhexloader.h>

#include <stdio.h>

int program_pic16f87x(const QString &target_mcu, const QString &source_project, const QString &hex_file)
{
    return -1000;
}

int program_picds30(const QString &target_mcu, const QString &source_project, const QString &hex_file)
{
    return -1000;
}

int program_picds33(const QString &target_mcu, const QString &source_project, const QString &hex_file)
{
    QHexLoader hex;
    int ret;

    PICPROG_HANDLE prg_handle = picprog_open();

    if (prg_handle == NULL)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("PicProg programator couldn\'t be detected."));
        return -331;
    }

    picprog_ds_enter_icsp(prg_handle);

    uint16_t app_id = picprog_ds_read_application_id(prg_handle);
    if (app_id != 0xCB)
    {

    }

    //uint32_t sanity = picprog_ds_sanity_check(prg_handle);

    picprog_ds_bulk_erase(prg_handle);

    QByteArray buffer;
    buffer.resize(128 * 1024); // 128KB
    memset(buffer.data(), 0xff, buffer.length());

    if (hex.load(hex_file))
    {
        QList<QHexRow> data = hex.data();

        // chip erase

        foreach(QHexRow row, data)
        {
            QByteArray segment = row.m_Data;

            if ((row.m_Type != 0))
            {
                break;
            }

            memcpy(buffer.data() + row.m_Address, segment.constData(), segment.length());
        }

        int firmware_size = 0;

        for(int pos = buffer.length() - 1; pos >= 0; pos--)
        {
            if ((uint8_t)buffer.at(pos) != 0xff)
            {
                firmware_size = pos + 1;
                break;
            }
        }

        int prg_segments = firmware_size / 192;
        if (firmware_size % 192)
        {
            prg_segments++;
        }

        for(int temp = 0; temp < 2048; temp++)
        {
            buffer[temp] = (uint8_t)temp;
        }

        buffer.resize(prg_segments * 192);

        for(int prg_segment = 0; prg_segment < prg_segments; prg_segment++)
        {
            picprog_ds_write_program(prg_handle, (uint16_t *)buffer.constData() + (prg_segment * (192 / 2)), prg_segment * (192 / 2));
            Sleep(10);
        }
    }
    else
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Corrupted hex file."));
        ret = -332;
    }

    QByteArray verify_buffer;
    verify_buffer.resize(buffer.size());
    memset(verify_buffer.data(), 31, verify_buffer.length());

    picprog_ds_read_program(prg_handle, (uint16_t *)verify_buffer.data(), 0, buffer.size() / (2 * 6));

    if (buffer != verify_buffer)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Verify failed."));
        ret = -333;
    }

    picprog_ds_exit_icsp(prg_handle);

    picprog_close(prg_handle);

    return ret;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString target_mcu;
    QString source_project;
    QString hex_file;

    QStringList args = app.arguments();

    args.takeFirst();

    if (args.count() == 0)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Usage qpicprog.exe -d device_name <-p project_file> hex_file."));
        return -1;
    }

    hex_file = args.takeLast();

    for (int index = 0; index < args.count() - 1; index++)
    {
        const QString &param = args.at(index);

        if (param == "-d")
        {
            target_mcu = args.at(index + 1);
            index++;
            continue;
        }

        if (param == "-p")
        {
            source_project = args.at(index + 1);
            index++;
            continue;
        }

        QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Warning, unknown argument: %1").arg(param));
    }

    if (target_mcu.isEmpty())
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Device name is not specified."));
        return -2;
    }

    if (target_mcu.startsWith("PIC16F87", Qt::CaseInsensitive))
    {
        program_pic16f87x(target_mcu, source_project, hex_file);
    }
    else if (target_mcu.startsWith("PICDS30", Qt::CaseInsensitive))
    {
        program_picds30(target_mcu, source_project, hex_file);
    }
    else if (target_mcu.startsWith("PICDS33", Qt::CaseInsensitive))
    {
        program_picds33(target_mcu, source_project, hex_file);
    }
    else
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Unsupported device: %1.").arg(target_mcu));
        return -3;
    }

    return 0;
}
