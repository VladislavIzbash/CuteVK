#include "DialogListModel.h"

DialogListModel::DialogListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QImage emptyImage;
    QString name = QString::fromUtf8("Владислав Избаш ");

    for (int i = 0; i < 9; i++) {
        bool online = qrand() % 2;
        m_dialogs.append({emptyImage, name + QString::number(i), online});
    }
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
