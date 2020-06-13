#pragma once

#include <QAbstractListModel>
#include <QPixmap>

class ApiClient;

struct DialogEntry {
    QImage photo;
    QString name;
    bool online;
};

Q_DECLARE_METATYPE(DialogEntry);

class DialogListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DialogListModel(ApiClient *apiClient, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<DialogEntry> m_dialogs;
    ApiClient *m_apiClient;

};

