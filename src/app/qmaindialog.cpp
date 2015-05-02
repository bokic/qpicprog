#include "qmaindialog.h"
#include "ui_qmaindialog.h"

#include "qhexloader.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

QMainDialog::QMainDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::QMainDialog)
{
    ui->setupUi(this);

    //QAdvancedTimer::setup();

    /*m_LptPorts = QLptPort::enumLptPorts();

    quint32 l_Port = 0;

    if (m_LptPorts.size() != 1)
    {
        if (m_LptPorts.size() == 0)
        {
            QMessageBox::critical(NULL, tr("Error"), tr("No parallel ports found."));
            return;
        }

        // TODO: Implement port choose dialog.
    }

    if (QLptPort::grantLptPortAccess(m_LptPorts[l_Port].m_Address) == false)
    {
        QMessageBox::critical(NULL, tr("Error"), tr("Can't open parallel port."));
        return;
    }*/
}

QMainDialog::~QMainDialog()
{
    delete ui;
}

void QMainDialog::on_getPicVersionButton_clicked()
{

}

void QMainDialog::on_readPICAllButton_clicked()
{

}

void QMainDialog::on_readPICCodeButton_clicked()
{

}

void QMainDialog::on_readPICDataButton_clicked()
{

}

void QMainDialog::on_programAllFromFileFilenameLabel_linkActivated(QString link)
{
    if (link == "filename")
    {
        QFileDialog l_dlg;

        l_dlg.setFileMode(QFileDialog::ExistingFile);
        l_dlg.setDefaultSuffix("hex");
        l_dlg.setNameFilter("Intel HEX32 format(*.hex)");
        l_dlg.selectFile("*.hex");
        if (l_dlg.exec() == QDialog::Accepted)
        {
            QString l_file = l_dlg.selectedFiles().at(0);

            m_FileName = l_file;

            QFileInfo fileInfo(l_file);

            ui->programAllFromFileFilenameLabel->setText("<a href=\"filename\">Filename:</a> " + fileInfo.fileName());
            l_file = l_file.replace('/', "\\");
            ui->programAllFromFileFilenameLabel->setToolTip("<b>Filename:</b><br />" + l_file);
        }
    }
}

void QMainDialog::on_programAllFromFileButton_clicked()
{
    if (m_FileName == "")
        on_programAllFromFileFilenameLabel_linkActivated("filename");

    if (m_FileName == "")
        return;

    QHexLoader l_Hex;

    if (l_Hex.Load(m_FileName) != false)
    {
        QByteArray buf;
        buf.resize(128 * 1024); // TODO: Get len from programmer member.
        buf.fill(0xFF);

        for(int c = 0; c < l_Hex.m_Rows.length(); c++)
        {
            QHexRow l_Row = l_Hex.m_Rows.at(c);
            if (l_Row.m_Type == 0x00)
            {
                for(int c2 = 0; c2 < l_Row.m_Data.size(); c2++)
                {
                    buf[l_Row.m_Address + c2] = l_Row.m_Data.at(c2);
                }
            }
        }

        int size = l_Hex.m_Rows.length();
        size++;
    }
}

void QMainDialog::on_eraseProtectedButton_clicked()
{

}

void QMainDialog::on_eraseStandardButton_clicked()
{

}

void QMainDialog::on_disassembleButton_clicked()
{

}
