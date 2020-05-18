#include "themeloader.h"

ThemeLoader::ThemeLoader()
{

}

void ThemeLoader::loadTheme(QString filePath, QList<ThemeItem *> &theme)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)) {
        theme.clear();
        QString line;
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            line = stream.readLine();
            if (line.contains("WPS", Qt::CaseSensitive)) {
                file.close();
                return;
            }
            if (!line.isEmpty()) {
                int pos = line.indexOf('=');
                QString name = line.left(pos);
                QString color = line.right(line.size() - 1 - pos);
                ThemeItem *themeItem = new ThemeItem();
                themeItem->name = name;
                themeItem->setRawColor(color.toInt());
                theme.append(themeItem);
            }
        }
    }
    file.close();
}

void ThemeLoader::saveTheme(QString filePath, QList<ThemeItem *> &theme)
{
    QFileInfo fi(filePath);
    makeDirIfNotExist(fi.absolutePath());
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)) {
        ThemeItem *item = nullptr;
        QTextStream stream(&file);
        for (int i = 0; i < theme.size(); i++) {
            item = theme.at(i);
            stream << item->name << "=" << item->getRawColor() << "\n";
        }
        file.close();
    }
}

void ThemeLoader::makeDirIfNotExist(const QString &path)
{
    QDir pathDir(path);
    if (!pathDir.exists()) {
        pathDir.mkpath(path);
    }
}

