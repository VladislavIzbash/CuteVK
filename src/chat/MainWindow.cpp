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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
    , m_loginView(new QWebEngineView)
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

    connect(ui->actionLogin, &QAction::triggered, this, &MainWindow::openLoginWindow);
    connect(m_loginView, &QWebEngineView::urlChanged, this, &MainWindow::extractToken);

    m_loginView->setFixedSize(800, 500);
    m_loginView->setWindowTitle(tr("Authorization"));
    m_loginView->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
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

void MainWindow::extractToken(const QUrl& url)
{
    QString fixed = url.toString().replace("#", "?");
    QUrlQuery query((QUrl(fixed)));

    QString token = query.queryItemValue("access_token");

    if (!token.isNull()) {
        qDebug() << token;
        m_loginView->hide();
    }
}

