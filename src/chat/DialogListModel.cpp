#include "DialogListModel.h"

DialogListModel::DialogListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QImage emptyImage;

    m_dialogs.append({emptyImage, QString::fromUtf8("Владислав Избаш"), true});
    m_dialogs.append({emptyImage, "Vladislav2", false});
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
