#include "DialogListModel.h"

#include "src/api/ApiClient.h"

#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

const int FETCH_COUNT = 12;
const int UPDATE_INTERVAL = 20;

DialogListModel::DialogListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_updateTimer(new QTimer(this))
{
    m_updateTimer->start(UPDATE_INTERVAL * 1000);
}

int DialogListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_conversationList.count();
}

QVariant DialogListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() && role != Qt::DisplayRole)
        return QVariant();

    QVariant data;
    data.setValue(m_conversationList.at(index.row()));

    return data;
}

bool DialogListModel::isFetchingMore() const
{
    return m_isFetching;
}

void DialogListModel::reset()
{
    beginResetModel();
    m_conversationList.clear();
    endResetModel();
}

void DialogListModel::fetchMore(const QModelIndex& parent)
{
    if(parent.isValid())
        return;

    if (m_isFetching == false && m_apiClient != nullptr) {
        m_apiClient->getConversations(m_conversationList.count(), FETCH_COUNT);

        m_isFetching = true;
    }
}

bool DialogListModel::canFetchMore(const QModelIndex& parent) const
{
    if (parent.isValid() || m_isFetching || m_apiClient == nullptr)
        return false;

    return m_conversationList.count() < m_totalDialogs;
}

void DialogListModel::addLoadedConversations(const QVector<Conversation>& conversations, int total)
{
    m_totalDialogs = total;

    beginInsertRows(QModelIndex(), m_conversationList.count(), m_conversationList.count() + conversations.count());
    m_conversationList.append(conversations);
    endInsertRows();

    m_isFetching = false;
    emit dataChanged(index(m_conversationList.count()), index(m_conversationList.count() + conversations.count()));
}

void DialogListModel::updateConversations(const QMap<int, bool> &statusMap)
{
    for (Conversation& conv : m_conversationList) {
        conv.online = statusMap.value(conv.id);
    }

    emit dataChanged(index(0), index(m_conversationList.count()));
}

void DialogListModel::onApiError()
{
    m_isFetching = false;
}

ApiClient *DialogListModel::apiClient() const
{
    return m_apiClient;
}

void DialogListModel::setApiClient(ApiClient *apiClient)
{
    m_apiClient = apiClient;

    connect(m_apiClient, &ApiClient::getConversationsDone, this, &DialogListModel::addLoadedConversations);
    connect(m_apiClient, &ApiClient::getConversationStatusDone, this, &DialogListModel::updateConversations);
    connect(m_apiClient, &ApiClient::apiError, this, &DialogListModel::onApiError);

    connect(m_updateTimer, &QTimer::timeout, m_apiClient, &ApiClient::getConversationStatus);
}
