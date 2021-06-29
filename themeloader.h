#ifndef THEMELOADER_H
#define THEMELOADER_H

#include <QList>
#include <QTextStream>
#include <QColor>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QDataStream>
#include <QFile>

#include "themeitem.h"

class ThemeLoader
{
    QString mWallpaperPath = "";

    public:
        ThemeLoader();
        void loadTheme(QString filePath, QList<ThemeItem*> &theme);
        void saveTheme(QString filePath, QList<ThemeItem*> &theme, QString imagePath = 0);
        QString wallpaperPath();
        void deleteTempImageWallpaper(QString wallpaper);

    private:
        void makeDirIfNotExist(const QString &path);
        void getImageFile(QString filePath, qint64 startPos);
        void saveImageTheme(QString filePath, QString imagePath);
};

#endif // THEMELOADER_H
