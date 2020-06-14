#pragma once

#include "src/api/ApiClient.h"

#include <QAbstractListModel>
#include <QPixmap>

class DialogListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DialogListModel(ApiClient* apiClient, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool isFetchingMore() const;

protected:
    void fetchMore(const QModelIndex& parent) override;

    bool canFetchMore(const QModelIndex& parent) const override;

private slots:
    void addLoadedConversations(const QVector<Conversation>& conversations, int total);

private:
    QVector<Conversation> m_conversationList;
    ApiClient* m_apiClient;
    bool m_isFetching = false;
    int m_totalDialogs = 100;
};

