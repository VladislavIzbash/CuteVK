#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include "DialogEntryDelegate.h"
#include "DialogListModel.h"
#include "../api/ApiClient.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QImageReader>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QDebug>
#include <QUrlQuery>

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
    , loginView(new QWebEngineView)
{
    ui->setupUi(this);
    QLabel* statusIcon = new QLabel(ui->statusbar);
    statusIcon->setPixmap(QIcon::fromTheme("dialog-error").pixmap(15, 15));

    QWidget* statusBarWidget = new QWidget(ui->statusbar);
    QHBoxLayout* hLayout = new QHBoxLayout;

    hLayout->addWidget(statusIcon);
    hLayout->addWidget(new QLabel(tr("Logged out")));

    statusBarWidget->setLayout(hLayout);

    ui->statusbar->addWidget(statusBarWidget);

    QImageReader imageReader(":/images/photo_placeholder");

    ApiClient *apiClient = new ApiClient("442f8df8d8777ce8efa688530c4d8ef3f503dd81d5a3dacd759ff770ed5730848a2c78d176c2ed417be38", this);
    DialogListModel *dialogModel = new DialogListModel(apiClient, this);

    ui->dialogView->setModel(dialogModel);
    ui->dialogView->setItemDelegate(new DialogEntryDelegate(imageReader.read()));

    connect(ui->actionLogin, &QAction::triggered, this, &ChatWindow::openLoginWindow);
    connect(loginView, &QWebEngineView::urlChanged, this, &ChatWindow::extractToken);
}

ChatWindow::~ChatWindow()
{
    delete ui;
    delete loginView;
}

void ChatWindow::openLoginWindow()
{
    loginView->setFixedSize(800, 500);
    loginView->setWindowTitle(tr("Authorization"));
    //loginView->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    loginView->load(QUrl("https://oauth.vk.com/authorize?client_id=2685278&scope=friends,photos,audio,video,status,wall,groups,messages,offline&redirect_uri=https://api.vk.com/blank.html&display=page&response_type=token&v=5.60"));
    loginView->show();
}

void ChatWindow::extractToken(const QUrl &url)
{
    QString fixed = url.toString().replace("#", "?");
    QUrlQuery query((QUrl(fixed)));

    QString token = query.queryItemValue("access_token");

    if (!token.isNull()) {
        qDebug() << token;
        loginView->hide();
    }
}

