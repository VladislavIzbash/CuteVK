#include "chat/MainWindow.h"

#include <QApplication>
#include <QTranslator>
#include <QtWebEngine>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load(":/translations/" + QLocale::system().name());
    app.installTranslator(&translator);

    QCoreApplication::setOrganizationName("CuteVK");
    QCoreApplication::setApplicationName("CuteVK");

    MainWindow window;
    window.show();

    return app.exec();
}
