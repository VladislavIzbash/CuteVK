#include "chat/ChatWindow.h"

#include "api/ApiClient.h"

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

    ChatWindow window;
    window.show();

    return app.exec();
}
