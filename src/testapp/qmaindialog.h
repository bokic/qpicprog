#ifndef QMAINDIALOG_H
#define QMAINDIALOG_H

#include <QtGui/QDialog>


namespace Ui
{
    class QMainDialog;
}

class QMainDialog : public QDialog
{
    Q_OBJECT

public:
    QMainDialog(QWidget *parent = 0);
    ~QMainDialog();

private:
    Ui::QMainDialog *ui;

    QString m_FileName;
    //QList<QLptDevice> m_LptPorts;

private slots:
    void on_disassembleButton_clicked();
    void on_eraseStandardButton_clicked();
    void on_eraseProtectedButton_clicked();
    void on_programAllFromFileButton_clicked();
    void on_programAllFromFileFilenameLabel_linkActivated(QString link);
    void on_readPICDataButton_clicked();
    void on_readPICCodeButton_clicked();
    void on_readPICAllButton_clicked();
    void on_getPicVersionButton_clicked();
};

#endif // QMAINDIALOG_H
