#include "themetablemodel.h"

ThemeTableModel::ThemeTableModel(QObject *parent, QString appPath)
    : QAbstractTableModel(parent)
{
    mAppPath = appPath;
}

void ThemeTableModel::assignData(QList<ThemeItem *> *atheme)
{
    beginResetModel();
    theme = atheme;
    size = theme->size();
    endResetModel();
}

void ThemeTableModel::refreshData()
{
    beginResetModel();
    size = theme->size();
    endResetModel();
}

int ThemeTableModel::rowCount(const QModelIndex &parent) const
{
    return size;
}

int ThemeTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant ThemeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    ThemeItem *ti = theme->at(index.row());
    if (role == Qt::DisplayRole && index.column() == 1) {
        return ti->name;
    }
    if (role == Qt::DecorationRole && index.column() == 2) {
        QString imagePath = mAppPath + "/views/" + ti->name + ".png";
        QFileInfo file(imagePath);
        if (file.exists()) {
            return QPixmap(imagePath);
        } else {
            return QVariant();
        }
    }
    if (role == Qt::BackgroundRole && index.column() == 0) {
        return QBrush(ti->getColor());
    }
    return QVariant();
}

QVariant ThemeTableModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    if (nRole != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        switch (nSection) {
            case 0:
                return QString(tr("Color"));
            case 1:
                return QString(tr("View name"));
            case 2:
                return QString(tr("Where color used"));
        }
    }
    if (orientation == Qt::Vertical) {
        if (theme->at(nSection)->isModified()) {
            return QString(QString::number(nSection + 1) + " *");
        } else {
            return QString(QString::number(nSection + 1));
        }
    }
    return QVariant();
}

bool ThemeTableModel::setData(const QModelIndex &index, const QVariant &value, int nRole)
{
    return false;
}

ThemeItem *ThemeTableModel::getByIndex(const QModelIndex &index)
{
    return theme->at(index.row());
}
