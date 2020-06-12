#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include <QLabel>
#include <QHBoxLayout>

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
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
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

