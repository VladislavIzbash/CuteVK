#include "chat/ChatWindow.h"

#include <QApplication>
#include <QtDebug>
#include <QTranslator>
#include <QtWebEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load(":/translations/" + QLocale::system().name());
    app.installTranslator(&translator);

    ChatWindow w;
    w.show();

    return app.exec();
}
