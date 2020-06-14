#pragma once

#include <QAbstractListModel>
#include <QPixmap>

class ApiClient;

struct DialogEntry {
    QImage photo;
    QString name;
    bool online;
    bool isChat;
};

Q_DECLARE_METATYPE(DialogEntry);

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
    void addLoadedDialogs(const QVector<DialogEntry>& dialogs, int total);

private:
    QVector<DialogEntry> m_dialogList;
    ApiClient* m_apiClient;
    bool m_isFetching = false;
    int m_totalDialogs = 100;
};

