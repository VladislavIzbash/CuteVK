#include "DialogListModel.h"

#include "src/api/ApiClient.h"

#include <QDebug>

const int BATCH_SIZE = 12;

DialogListModel::DialogListModel(ApiClient *apiClient, QObject *parent)
    : QAbstractListModel(parent)
    , m_apiClient(apiClient)
{
    m_apiClient->queryConversationsAsync(0, BATCH_SIZE);
    connect(m_apiClient, &ApiClient::queryConversationsDone, this, &DialogListModel::addLoadedDialogs);
}


int DialogListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dialogs.count();
}

QVariant DialogListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() && role != Qt::DisplayRole)
        return QVariant();

    QVariant data;
    data.setValue(m_dialogs.at(index.row()));

    return data;
}


void DialogListModel::fetchMore(const QModelIndex &parent)
{
    if(parent.isValid())
        return;

    if (m_loading == false) {
        m_apiClient->queryConversationsAsync(m_dialogs.count(), BATCH_SIZE);
        emit loadingStarted();
        m_loading = true;
    }
}

bool DialogListModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || m_loading)
        return false;

    return m_dialogs.count() < m_totalDialogs;
}

void DialogListModel::addLoadedDialogs(QVector<DialogEntry> dialogs, int total)
{
    m_totalDialogs = total;

    beginInsertRows(QModelIndex(), m_dialogs.count(), m_dialogs.count() + dialogs.count());
    m_dialogs.append(dialogs);
    endInsertRows();

    emit loadingDone();
    m_loading = false;
}
