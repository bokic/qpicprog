#include <QApplication>
#include <QStringList>
#include <QMessageBox>

#include <advanced_delay.h>
#include <picprog_16f87x.h>
#include <picprog_ds33_ds24.h>
#include <picprog_ds30.h>
#include <picprog.h>

#include <qhexloader.h>

#include <stdio.h>

void picds30_read_test()
{
    adv_delay_setup();

    PICPROG_HANDLE prg_handle = picprog_open();

    if (prg_handle != NULL)
    {
        picprog_ds30_enter_icsp(prg_handle);

        uint16_t buffer[(8 * 6) + 1];

        memset(buffer, 1, sizeof(buffer));

        //picprog_ds30_bulk_erase(prg_handle);

        //picprog_ds30_write_program(prg_handle, buffer, 0);

        uint16_t config[12];
        picprog_ds30_read_config_memory(prg_handle, config);
        //picprog_ds30_read_program(prg_handle, buffer, 0, 8);

        /*if (buffer[0] == 0x0101)
        {
            printf("Successfull write and read operations.\n");
        }
        else
        {
            printf("Write and read operations FAILED.\n");
        }*/

        picprog_ds30_exit_icsp(prg_handle);

        picprog_close(prg_handle);
    }
}

void picds33_read_test()
{
    PICPROG_HANDLE prg_handle = picprog_open();

    if (prg_handle != NULL)
    {
        picprog_ds33_ds24_enter_icsp(prg_handle);

        uint16_t buffer[1024];

        memset(buffer, 0, sizeof(buffer));

        //picprog_ds33_ds24_bulk_erase(prg_handle);

        picprog_ds33_ds24_read_program(prg_handle, buffer, 0, 1);

        picprog_ds33_ds24_exit_icsp(prg_handle);

        picprog_close(prg_handle);
    }
}

void picds30_test()
{
    PICPROG_HANDLE prg_handle = picprog_open();

    if (prg_handle != NULL)
    {
        picprog_ds30_enter_icsp(prg_handle);

        uint16_t app_id = picprog_ds30_read_application_id(prg_handle);

        if (app_id != 0xBB)
        {
            //
        }

        picprog_close(prg_handle);
    }
}

void picds33_test()
{
    PICPROG_HANDLE prg_handle = picprog_open();

    if (prg_handle != NULL)
    {
        picprog_ds33_ds24_enter_icsp(prg_handle);

        uint16_t app_id = picprog_ds33_ds24_read_application_id(prg_handle);

        if (app_id != 0xBB)
        {
            //
        }

        picprog_close(prg_handle);
    }

}

void picds30_flash_led()
{
    PICPROG_HANDLE handle = picprog_open();

    if (handle != NULL)
    {
        picprog_ds30_enter_icsp(handle);

        picprog_ds30_write_command(handle, 0x040200); // GOTO 0x200
        picprog_ds30_write_command(handle, 0x040200); // GOTO 0x200
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xA982C8); // BCLR	TRISBbits, #4
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xAA82CC); // BTG	LATBbits, #4
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xAA82CC); // BTG	LATBbits, #4
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0xAA82CC); // BTG	LATBbits, #4
        picprog_ds30_write_command(handle, 0x000000); // NOP
        picprog_ds30_write_command(handle, 0x000000); // NOP

        picprog_ds30_exit_icsp(handle);

        picprog_close(handle);
    }
}

void picds33_flash_led()
{
    PICPROG_HANDLE handle = picprog_open();

    if (handle != NULL)
    {
        picprog_ds33_ds24_enter_icsp(handle);

        picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
        picprog_ds33_ds24_write_command(handle, 0x040200); // GOTO 0x200
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xA982C8); // BCLR	TRISBbits, #4
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xAA82CC); // BTG	LATBbits, #4
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xAA82CC); // BTG	LATBbits, #4
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0xAA82CC); // BTG	LATBbits, #4
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP
        picprog_ds33_ds24_write_command(handle, 0x000000); // NOP

        picprog_ds33_ds24_exit_icsp(handle);

        picprog_close(handle);
    }
}

/*int program_pic16f87x(const QString &target_mcu, const QString &source_project, const QString &hex_file)
{
    return -1000;
}

int program_picds30(const QString &target_mcu, const QString &source_project, const QString &hex_file)
{
    return -1000;
}*/

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

    if (hex.load(hex_file))
    {
        QList<QHexRow> data = hex.data();

        /// chip erase - with reset
        picprog_ds33_ds24_enter_icsp(prg_handle);
        picprog_ds33_ds24_bulk_erase(prg_handle);
        picprog_ds33_ds24_exit_icsp(prg_handle);


        picprog_ds33_ds24_enter_icsp(prg_handle);

        /// TODO: Test code for delete
        //uint16_t tttt[24];
        //uint8_t tttt[48];
        //memset(tttt, 0x22, sizeof(tttt));
        //picprog_ds33_ds24_read_config_memory(prg_handle, (uint16_t *)tttt);
        /// delete til here

        /// setup code buffer
        QByteArray buffer;
        buffer.resize(128 * 1024); // 128KB
        memset(buffer.data(), 0xff, buffer.length());

        foreach(QHexRow row, data)
        {
            QByteArray segment = row.m_Data;

            if ((row.m_Type != 0))
            {
                break;
            }

            memcpy(buffer.data() + row.m_Address, segment.constData(), segment.length());
        }

        /// Program code memory
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

        buffer.resize(prg_segments * 192);

        for(int prg_segment = 0; prg_segment < prg_segments; prg_segment++)
        {
            picprog_ds33_ds24_write_program(prg_handle, (uint8_t *)buffer.constData() + (prg_segment * 256), prg_segment * 128);
            adv_delaym(10);
        }

        //uint16_t app_id = picprog_ds33_ds24_read_app_id(prg_handle);

        /// Verify program code
        /*QByteArray verify_buffer;
        verify_buffer.resize(buffer.size());
        verify_buffer.fill(0x11);

        picprog_ds33_ds24_read_program(prg_handle, (uint16_t *)verify_buffer.data(), 0, buffer.size() / (2 * 6));

        if (buffer != verify_buffer)
        {
            QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Verify code memory failed."));
            ret = -334;
        }*/

        /// Program & Verify config memory
        if (data.count() > 2)
        {
            QByteArray config_data = data.at(data.count() - 2).m_Data;

            while(config_data.length() < 48)
            {
                config_data.append('\0');
            }

            if (config_data.length() != 48)
            {
                config_data.resize(48);
            }

            picprog_ds33_ds24_write_config_memory(prg_handle, (uint16_t *)config_data.data());

            QByteArray verify_config_data;

            verify_config_data.resize(48);

            picprog_ds33_ds24_read_config_memory(prg_handle, (uint16_t *)verify_config_data.data());

            if (config_data != verify_config_data)
            {
                QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Verify config memory failed."));
                ret = -332;
            }
        }

        picprog_ds33_ds24_exit_icsp(prg_handle);

        picprog_close(prg_handle);
    }
    else
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Corrupted hex file."));
        ret = -333;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    picds30_read_test(); return 0;
    //picds33_read_test(); return 0;
    //picds30_test();return 0;
    //picds33_test();return 0;
    //picds30_flash_led(); return 0;
    //picds33_flash_led();return 0;

    QApplication app(argc, argv);

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
        //program_pic16f87x(target_mcu, source_project, hex_file);
    }
    else if ((target_mcu.startsWith("PICDS30", Qt::CaseInsensitive))||(target_mcu.startsWith("PIC30", Qt::CaseInsensitive)))
    {
        //program_picds30(target_mcu, source_project, hex_file);
    }
    else if ((target_mcu.startsWith("PICDS33", Qt::CaseInsensitive))||(target_mcu.startsWith("PIC33", Qt::CaseInsensitive)))
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
