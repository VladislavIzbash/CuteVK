#pragma once

#include <QAbstractItemDelegate>

class DialogEntryDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit DialogEntryDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const QImage m_defaultPhoto;
};

