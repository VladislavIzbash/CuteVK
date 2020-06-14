#include "DialogListModel.h"

#include "src/api/ApiClient.h"

#include <QDebug>

const int FETCH_COUNT = 12;

DialogListModel::DialogListModel(ApiClient* apiClient, QObject* parent)
    : QAbstractListModel(parent)
    , m_apiClient(apiClient)
{
    connect(m_apiClient, &ApiClient::queryConversationsDone, this, &DialogListModel::addLoadedDialogs);
}


int DialogListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_dialogList.count();
}

QVariant DialogListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() && role != Qt::DisplayRole)
        return QVariant();

    QVariant data;
    data.setValue(m_dialogList.at(index.row()));

    return data;
}

bool DialogListModel::isFetchingMore() const
{
    return m_isFetching;
}

void DialogListModel::fetchMore(const QModelIndex& parent)
{
    if(parent.isValid())
        return;

    if (m_isFetching == false) {
        m_apiClient->queryConversationsAsync(m_dialogList.count(), FETCH_COUNT);

        m_isFetching = true;
    }
}

bool DialogListModel::canFetchMore(const QModelIndex& parent) const
{
    if (parent.isValid() || m_isFetching)
        return false;

    return m_dialogList.count() < m_totalDialogs;
}

void DialogListModel::addLoadedDialogs(const QVector<DialogEntry>& dialogs, int total)
{
    m_totalDialogs = total;

    beginInsertRows(QModelIndex(), m_dialogList.count(), m_dialogList.count() + dialogs.count());
    m_dialogList.append(dialogs);
    endInsertRows();

    m_isFetching = false;
    emit dataChanged(index(m_dialogList.count()), index(m_dialogList.count() + dialogs.count()));
}
