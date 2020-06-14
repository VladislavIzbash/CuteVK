#include "DialogEntryDelegate.h"

#include "DialogListModel.h"

#include <QPainter>
#include <QDebug>
#include <QImageReader>

const int PHOTO_SIZE = 50;
const int INDICATOR_WIDTH = 5;

DialogEntryDelegate::DialogEntryDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
    , m_defaultPhoto(QImageReader(":/images/photo_placeholder").read())
{}

void DialogEntryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.data().canConvert<DialogEntry>())
        return;

    DialogEntry dialog = qvariant_cast<DialogEntry>(index.data());

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(option.rect.x(), option.rect.y());

    // Highlight
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(0, 0, option.rect.width(), PHOTO_SIZE, option.palette.highlight());
    }

    // Photo
    painter->drawImage(0, 0, dialog.photo.isNull() ? m_defaultPhoto : dialog.photo);

    // Name centered
    QFont font = painter->font();
    font.setPixelSize(15);
    painter->setFont(font);
    painter->drawText(PHOTO_SIZE + 20, 0, option.rect.width() - PHOTO_SIZE - INDICATOR_WIDTH, PHOTO_SIZE, Qt::AlignVCenter, dialog.name);

    // Online indicator
    if (!dialog.isChat) {
        painter->fillRect(option.rect.width() - INDICATOR_WIDTH, 0, INDICATOR_WIDTH, option.rect.height(), dialog.online ? Qt::green : Qt::gray);
    }

    painter->setPen(option.palette.Foreground);

    painter->restore();
}

QSize DialogEntryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);

    return QSize(PHOTO_SIZE + INDICATOR_WIDTH, PHOTO_SIZE);
}
