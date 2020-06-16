#include "DialogListWidget.h"

#include "DialogListModel.h"
#include "DialogEntryDelegate.h"

#include "src/api/ApiClient.h"

#include <QListView>
#include <QVBoxLayout>
#include <QDebug>
#include <KF5/KWidgetsAddons/KBusyIndicatorWidget>
#include <QSettings>
#include <QCoreApplication>
#include <QStatusTipEvent>

DialogListWidget::DialogListWidget(QWidget* parent)
    : QListView(parent)
    , m_fetchmoreSpinner(new KBusyIndicatorWidget(this))
    , m_loadingSpinner(new KBusyIndicatorWidget(this))
{
    ApiClient* apiClient = new ApiClient(this);
    QSettings settings;

    if (settings.contains("token")) {
        apiClient->setToken(settings.value("token").toString());
    }

    DialogListModel* dialogModel = new DialogListModel;
    dialogModel->setApiClient(apiClient);

    setModel(dialogModel);
    setItemDelegate(new DialogEntryDelegate);

    m_loadingSpinner->setFixedSize(50, 50);

    m_fetchmoreSpinner->setFixedSize(30, 30);
    m_fetchmoreSpinner->hide();

    setSpacing(2);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_loadingSpinner, 0, Qt::AlignCenter);
    layout->addWidget(m_fetchmoreSpinner, 0, Qt::AlignHCenter | Qt::AlignBottom);
    setLayout(layout);

    connect(dialogModel, &DialogListModel::dataChanged, m_loadingSpinner, &KBusyIndicatorWidget::hide);
    connect(apiClient, &ApiClient::apiError, [=] (const QString& message) {
        m_loadingSpinner->hide();

        QCoreApplication::postEvent(parent, new QStatusTipEvent(message));
        qWarning() << message;
    });
}

void DialogListWidget::paintEvent(QPaintEvent* event)
{
    QListView::paintEvent(event);

    if (!indexAt(QPoint(0, height() - 20)).isValid()
            && qobject_cast<DialogListModel*>(model())->isFetchingMore()
            && m_loadingSpinner->isHidden()) {

        m_fetchmoreSpinner->show();

    } else {
        m_fetchmoreSpinner->hide();
    }
}
