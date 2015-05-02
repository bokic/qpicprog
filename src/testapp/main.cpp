#include <QtGui/QApplication>
#include "qmaindialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainDialog dlg;
    dlg.show();
    return app.exec();
}
