#include "DialogListWidget.h"

#include "DialogListModel.h"
#include "DialogEntryDelegate.h"

#include "src/api/ApiClient.h"

#include <QListView>
#include <QVBoxLayout>
#include <KF5/KWidgetsAddons/KBusyIndicatorWidget>
#include <QMovie>

DialogListWidget::DialogListWidget(QWidget *parent)
    : QWidget(parent)
    , m_dialogList(new QListView(this))
    , m_loadingSpinner(new KBusyIndicatorWidget(this))
{
    ApiClient *apiClient = new ApiClient("442f8df8d8777ce8efa688530c4d8ef3f503dd81d5a3dacd759ff770ed5730848a2c78d176c2ed417be38", this);

    DialogListModel *dialogModel = new DialogListModel(apiClient);
    connect(dialogModel, &DialogListModel::loadingDone, this, &DialogListWidget::hideSpinner);
    connect(dialogModel, &DialogListModel::loadingStarted, this, &DialogListWidget::showSpinner);

    m_dialogList->setModel(dialogModel);
    m_dialogList->setItemDelegate(new DialogEntryDelegate);
    m_dialogList->setSpacing(2);
    m_dialogList->hide();

    m_loadingSpinner->setFixedSize(50, 50);
    m_loadingSpinner->setGeometry(m_dialogList->width() / 2, m_dialogList->height(), 50, 50);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_dialogList);
    setLayout(layout);
}

void DialogListWidget::hideSpinner()
{
    m_loadingSpinner->hide();
    m_dialogList->show();
}

void DialogListWidget::showSpinner()
{
    m_loadingSpinner->setFixedSize(30, 30);
    m_loadingSpinner->setGeometry(m_dialogList->width() / 2 - 15, m_dialogList->height() - 30, 30, 30);
    m_loadingSpinner->show();
}
