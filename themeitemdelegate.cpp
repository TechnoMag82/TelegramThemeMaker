#include "themeitemdelegate.h"

ThemeItemDelegate::ThemeItemDelegate(QObject *pobj) : QStyledItemDelegate(pobj)
{

}

void ThemeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected && index.column() == 0) {
        if (index.data(Qt::BackgroundRole).canConvert<QBrush>()) {
            QBrush background = qvariant_cast<QBrush>(index.data(Qt::BackgroundRole));
            painter->save();
            painter->fillRect(option.rect, background);
            painter->restore();
        }
    } else if (option.state & QStyle::State_Selected && index.column() == 2) {
        if (index.data(Qt::DecorationRole).canConvert<QPixmap>()) {
            QPixmap background = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));
            QRect target(option.rect.left() + 3,
                         option.rect.top(),
                         background.width(),
                         background.height());
            QRect source(0,
                         (background.height() / 2 - option.rect.height() / 2),
                         background.width(),
                         background.height());
            painter->save();
            painter->drawPixmap(target,
                                background,
                                source);
            painter->restore();
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
