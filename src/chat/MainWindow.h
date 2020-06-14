#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class QWebEngineView;
class ApiClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openLoginWindow();

    void extractToken(const QUrl& url);

private:
    Ui::ChatWindow* ui;
    QWebEngineView* m_loginView;
};
