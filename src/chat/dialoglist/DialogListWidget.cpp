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
    connect(dialogModel, &DialogListModel::loadingDone, this, &DialogListWidget::displayList);

    m_dialogList->setModel(dialogModel);
    m_dialogList->setItemDelegate(new DialogEntryDelegate);
    m_dialogList->setSpacing(2);
    m_dialogList->hide();

    m_loadingSpinner->setFixedSize(50, 50);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_dialogList);
    layout->addWidget(m_loadingSpinner);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);
}

void DialogListWidget::displayList()
{
    m_loadingSpinner->hide();
    m_dialogList->show();
}
