#ifndef THEMETABLEMPDEL_H
#define THEMETABLEMPDEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDebug>
#include <QString>
#include <QObject>
#include <QMap>
#include <QBrush>
#include <QColor>
#include <QFileInfo>

#include "themeitem.h"

class ThemeTableModel : public QAbstractTableModel
{
    Q_OBJECT
    private:
        int size;
        QList<ThemeItem*> *theme = nullptr;
        QString mAppPath;

    public:
        explicit ThemeTableModel(QObject *parent, QString appPath);

        void assignData(QList<ThemeItem*> *atheme);
        void refreshData();

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        QVariant headerData(int nSection, Qt::Orientation orientation, int nRole) const;
        bool setData(const QModelIndex& index, const QVariant& value, int nRole);
        ThemeItem* getByIndex(const QModelIndex &index);
};

#endif // THEMETABLEMPDEL_H
