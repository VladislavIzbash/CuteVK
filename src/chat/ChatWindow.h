#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class QWebEngineView;
class ApiClient;

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
