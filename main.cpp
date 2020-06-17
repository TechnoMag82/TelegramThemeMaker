#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator myTranslator;
    QString locale = QLocale::system().name();
    myTranslator.load(QString(":/translations/main_" + locale.left(2)));
    app.installTranslator(&myTranslator);
    QCoreApplication::setOrganizationName("TechnoMag");
    QCoreApplication::setApplicationName("Telegram Theme Maker") ;
    QCoreApplication::setApplicationVersion("1.3");
    MainWindow w;
    w.show();

    return app.exec();
}
