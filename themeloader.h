#ifndef THEMELOADER_H
#define THEMELOADER_H

#include <QList>
#include <QTextStream>
#include <QColor>
#include <QFileInfo>
#include <QDir>

#include "themeitem.h"

class ThemeLoader
{
public:
    ThemeLoader();
    void loadTheme(QString filePath, QList<ThemeItem*> &theme);
    void saveTheme(QString filePath, QList<ThemeItem*> &theme);
private:
    void makeDirIfNotExist(const QString &path);
};

#endif // THEMELOADER_H
