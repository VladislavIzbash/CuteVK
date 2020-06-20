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
#include <QStateMachine>

DialogListWidget::DialogListWidget(QWidget* parent)
    : QWidget(parent)
    , m_stateMachine(new QStateMachine)
    , m_dialogList(new QListView)
    , m_loadingSpinner(new KBusyIndicatorWidget(this))
{
    DialogListModel* dialogModel = new DialogListModel;
    ApiClient* apiClient = new ApiClient(this);
    dialogModel->setApiClient(apiClient);

    QSettings settings;
    if (settings.contains("token")) {
        apiClient->setToken(settings.value("token").toString());
    }

    m_dialogList->setModel(dialogModel);
    m_dialogList->setItemDelegate(new DialogEntryDelegate);
    m_dialogList->setSpacing(2);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_dialogList);
    layout->addWidget(m_loadingSpinner, 0, Qt::AlignCenter);
    setLayout(layout);

    QState* loadingState = new QState;
    QState* readyState = new QState;
    QState* fetchingMoreState = new QState;

    loadingState->assignProperty(m_loadingSpinner, "minimumSize", QSize(50, 50));
    connect(loadingState, &QState::entered, m_dialogList, &QListView::hide);
    loadingState->addTransition(dialogModel, &DialogListModel::dataChanged, readyState);
    loadingState->addTransition(apiClient, &ApiClient::apiError, readyState);

    connect(readyState, &QState::entered, m_loadingSpinner, &KBusyIndicatorWidget::hide);
    connect(readyState, &QState::entered, m_dialogList, &QListView::show);
    readyState->addTransition(model(), &DialogListModel::fetchingMore, fetchingMoreState);

    fetchingMoreState->assignProperty(m_loadingSpinner, "minimumSize", QSize(20, 20));
    connect(fetchingMoreState, &QState::entered, m_loadingSpinner, &KBusyIndicatorWidget::show);
    connect(fetchingMoreState, &QState::entered, m_dialogList, &QListView::show);
    fetchingMoreState->addTransition(dialogModel, &DialogListModel::dataChanged, readyState);
    fetchingMoreState->addTransition(apiClient, &ApiClient::apiError, loadingState);

    m_stateMachine->addState(loadingState);
    m_stateMachine->addState(readyState);
    m_stateMachine->addState(fetchingMoreState);
    m_stateMachine->setInitialState(loadingState);
    m_stateMachine->start();

    connect(apiClient, &ApiClient::apiError, this, &DialogListWidget::printError);
}

DialogListModel* DialogListWidget::model()
{
    return static_cast<DialogListModel*>(m_dialogList->model());
}

void DialogListWidget::printError(const QString& message)
{
    QCoreApplication::postEvent(parent(), new QStatusTipEvent(message));
    qWarning() << message;
}



