#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include "DialogEntryDelegate.h"
#include "DialogListModel.h"

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

    ui->dialogView->setModel(new DialogListModel());
    ui->dialogView->setItemDelegate(new DialogEntryDelegate(imageReader.read()));

    connect(ui->actionLogin, SIGNAL(triggered()), this, SLOT(openLoginWindow()));
    connect(loginView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(extractToken(const QUrl&)));
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
    loginView->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    loginView->load(QUrl("https://oauth.vk.com/authorize?client_id=6700788&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.52"));
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

