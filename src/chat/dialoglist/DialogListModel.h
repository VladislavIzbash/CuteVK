#pragma once

#include "src/api/ApiClient.h"

#include <QAbstractListModel>
#include <QPixmap>

class DialogListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DialogListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void fetchMore(const QModelIndex& parent) override;

    bool isFetchingMore() const;

    void clear();

    ApiClient *apiClient() const;
    void setApiClient(ApiClient *apiClient);

protected:
    bool canFetchMore(const QModelIndex& parent) const override;

private slots:
    void addLoadedConversations(const QVector<Conversation>& conversations, int total);

    void updateConversations(const QMap<int, bool>& statusMap);

    void onApiError();

private:
    QVector<Conversation> m_conversationList;
    ApiClient* m_apiClient = nullptr;
    QTimer* m_updateTimer;
    bool m_isFetching = false;
    int m_totalDialogs = 100;
};

