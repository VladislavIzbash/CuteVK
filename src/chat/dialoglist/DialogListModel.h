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
    DialogListModel(ApiClient *apiClient, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    void fetchMore(const QModelIndex &parent) override;

    bool canFetchMore(const QModelIndex &parent) const override;

signals:
    void loadingStarted();
    void loadingDone();

private slots:
    void addLoadedDialogs(QVector<DialogEntry> dialogs, int total);

private:
    QVector<DialogEntry> m_dialogs;
    ApiClient *m_apiClient;
    bool m_loading = true;
    int m_totalDialogs = 0;

};

