#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
class QWebEngineView;
QT_END_NAMESPACE

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void openLoginWindow();

    void extractToken(const QUrl &url);

private:
    Ui::ChatWindow *ui;
    QWebEngineView *loginView;

};
