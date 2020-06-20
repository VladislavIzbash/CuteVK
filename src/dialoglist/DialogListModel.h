#pragma once

#include "src/api/ApiClient.h"

#include <QAbstractListModel>

class DialogListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DialogListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    ApiClient* apiClient() const;
    void setApiClient(ApiClient* apiClient);

    void reset();

protected:
    bool canFetchMore(const QModelIndex& parent) const override;

    void fetchMore(const QModelIndex& parent) override;

signals:
    void fetchingMore();

private slots:
    void addConversations(const QVector<Conversation>& conversations, int total);

    void updateConversations(const QMap<int, bool>& statusMap);

    void stopFetching();

private:
    QVector<Conversation> m_conversationList;
    ApiClient* m_apiClient = nullptr;
    QTimer* m_updateTimer;
    bool m_isFetching = false;
    int m_totalDialogs = 100;
};

