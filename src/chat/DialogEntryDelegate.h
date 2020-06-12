#pragma once

#include <QAbstractItemDelegate>

class DialogEntryDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    DialogEntryDelegate(QImage defaultPhoto, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    static const int PHOTO_SIZE = 50;
    static const int INDICATOR_WIDTH = 5;

    const QImage m_defaultPhoto;

};

