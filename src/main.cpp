#include "ChatWindow.h"

#include <QApplication>
#include <QtDebug>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load(":/CuteVK_" + QLocale::system().name() + ".qm");
    app.installTranslator(&translator);

    ChatWindow w;
    w.show();

    return app.exec();
}
