#include "DialogListModel.h"

#include "src/api/ApiClient.h"

DialogListModel::DialogListModel(ApiClient *apiClient, QObject *parent)
    : QAbstractListModel(parent)
    , m_apiClient(apiClient)
{
    m_apiClient->queryConversationsAsync(0, 10);
    connect(m_apiClient, &ApiClient::queryConversationsDone, this, [this] (QVector<DialogEntry> dialogs) {
       m_dialogs.append(dialogs);
       emit dataChanged(createIndex(0, 0), createIndex(10, 0));
       emit loadingDone();
    });
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
