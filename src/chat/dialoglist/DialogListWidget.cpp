#include "DialogListWidget.h"

#include "DialogListModel.h"
#include "DialogEntryDelegate.h"

#include "src/api/ApiClient.h"

#include <QListView>
#include <QVBoxLayout>
#include <QDebug>
#include <KF5/KWidgetsAddons/KBusyIndicatorWidget>

DialogListWidget::DialogListWidget(QWidget *parent)
    : QListView(parent)
    , m_fetchmoreSpinner(new KBusyIndicatorWidget(this))
    , m_loadingSpinner(new KBusyIndicatorWidget(this))
{
    ApiClient *apiClient = new ApiClient("442f8df8d8777ce8efa688530c4d8ef3f503dd81d5a3dacd759ff770ed5730848a2c78d176c2ed417be38", this);

    DialogListModel *dialogModel = new DialogListModel(apiClient);
    setModel(dialogModel);
    setItemDelegate(new DialogEntryDelegate);

    m_loadingSpinner->setFixedSize(50, 50);

    m_fetchmoreSpinner->setFixedSize(30, 30);
    m_fetchmoreSpinner->hide();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_loadingSpinner, 0, Qt::AlignCenter);
    layout->addWidget(m_fetchmoreSpinner, 0, Qt::AlignHCenter | Qt::AlignBottom);
    setLayout(layout);

    connect(dialogModel, &DialogListModel::dataChanged, m_loadingSpinner, &KBusyIndicatorWidget::hide);
}

void DialogListWidget::paintEvent(QPaintEvent *event)
{
    QListView::paintEvent(event);

    if (!indexAt(QPoint(0, height())).isValid()
            && qobject_cast<DialogListModel*>(model())->isFetchingMore()
            && m_loadingSpinner->isHidden()) {

        m_fetchmoreSpinner->show();

    } else {
        m_fetchmoreSpinner->hide();
    }
}
