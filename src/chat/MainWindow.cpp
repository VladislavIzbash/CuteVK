#include "MainWindow.h"
#include "ui_ChatWindow.h"

#include "dialoglist/DialogEntryDelegate.h"
#include "dialoglist/DialogListModel.h"

#include "src/api/ApiClient.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QImageReader>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QDebug>
#include <QUrlQuery>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
    , m_loginView(new QWebEngineView)
{
    ui->setupUi(this);

    m_loginView->setFixedSize(800, 500);
    m_loginView->setWindowTitle(tr("Authorization"));
    m_loginView->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);

    connect(ui->actionLogin, &QAction::triggered, this, &MainWindow::openLoginWindow);
    connect(ui->actionLogout, &QAction::triggered, this, &MainWindow::logout);
    connect(m_loginView, &QWebEngineView::urlChanged, this, &MainWindow::saveToken);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_loginView;
}

void MainWindow::openLoginWindow()
{
    m_loginView->load(QUrl("https://oauth.vk.com/authorize?client_id=2685278&scope=friends,photos,audio,video,status,wall,groups,messages,offline&redirect_uri=https://api.vk.com/blank.html&display=page&response_type=token&v=5.60"));
    m_loginView->show();
}

void MainWindow::saveToken(const QUrl& url)
{
    QString fixed = url.toString().replace("#", "?");
    QUrlQuery query((QUrl(fixed)));

    QString token = query.queryItemValue("access_token");

    if (!token.isNull()) {
        m_loginView->hide();

        QSettings settings;
        settings.setValue("token", token);

        DialogListModel* dialogModel = qobject_cast<DialogListModel*>(ui->dialogListWidget->model());
        dialogModel->apiClient()->setToken(token);
        dialogModel->reset();
    }
}

void MainWindow::logout()
{
    DialogListModel* dialogModel = qobject_cast<DialogListModel*>(ui->dialogListWidget->model());
    dialogModel->apiClient()->setToken("");
    dialogModel->reset();

    QSettings settings;
    settings.remove("token");
}

