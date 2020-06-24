#ifndef THEMEITEMDELEGATE_H
#define THEMEITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QRect>
#include <QLinearGradient>
#include <QPainter>

#include "themeitem.h"

class ThemeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    public:
        ThemeItemDelegate(QObject* pobj = 0);
        void paint(QPainter* painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};

#endif // THEMEITEMDELEGATE_H
