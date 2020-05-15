#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("TechnoMag");
    QCoreApplication::setApplicationName("Telegram Theme Maker") ;
    QCoreApplication::setApplicationVersion("1.0b");
    MainWindow w;
    w.show();

    return a.exec();
}
