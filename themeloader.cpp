#include "themeloader.h"

ThemeLoader::ThemeLoader()
{

}

void ThemeLoader::loadTheme(QString filePath, QList<ThemeItem *> &theme)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)) {
        theme.clear();
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line;
            line = stream.readLine();
            if (line.contains("WLS", Qt::CaseSensitive)) {
                file.close();
                return;
            }
            if (line.contains("WPS", Qt::CaseSensitive)) {
                qint64 startPos = stream.pos();
                file.close();
                getImageFile(filePath, startPos);
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

void ThemeLoader::saveTheme(QString filePath, QList<ThemeItem *> &theme, QString imagePath)
{
    QFileInfo fi(filePath);
    makeDirIfNotExist(fi.absolutePath());
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)) {
        ThemeItem *item = nullptr;
        QTextStream stream(&file);
        for (int i = 0; i < theme.size(); i++) {
            item = theme.at(i);
            if (item->name.contains("chat_wallpaper")
                    && !imagePath.isNull()
                    && !imagePath.isEmpty())
            {
                continue;
            }
            stream << item->name << "=" << item->getRawColor() << "\n";
        }
        file.close();
        if (!imagePath.isNull() && !imagePath.isEmpty()) {
            saveImageTheme(filePath, imagePath);
        }
    }
}

QString ThemeLoader::wallpaperPath()
{
    return mWallpaperPath;
}

void ThemeLoader::deleteWallpaper(QString wallpaper)
{
    if (wallpaper.startsWith("wallpaper-")) {
        QFile fi(wallpaper);
        fi.remove();
    }
}

void ThemeLoader::makeDirIfNotExist(const QString &path)
{
    QDir pathDir(path);
    if (!pathDir.exists()) {
        pathDir.mkpath(path);
    }
}

void ThemeLoader::getImageFile(QString filePath, qint64 startPos)
{
    QFile file(filePath);
    int sizeForRead = file.size() - startPos - 5;
    char *data = new char[sizeForRead];
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        int skipped = stream.skipRawData(startPos);
        if (skipped != -1) {
            int readed = stream.readRawData(data, sizeForRead);
            if (readed != -1) {
                mWallpaperPath = "wallpaper-" + QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss") + ".jpg";
                QFile imgFile(mWallpaperPath);
                if(imgFile.open(QIODevice::WriteOnly)) {
                    QDataStream out_stream(&imgFile);
                    out_stream.writeRawData(data, readed);
                    imgFile.close();
                }
            }
        }
    }
    file.close();
    delete data;
}

void ThemeLoader::saveImageTheme(QString filePath, QString imagePath)
{
    QFile imgFile(imagePath);
    int sizeImageForRead = imgFile.size();
    int sizeThemeForRead = imgFile.size();
    char *wps = "WPS\n";
    char *wpe = "\nWPE\n";
    char *imgData = new char[sizeImageForRead];
    if(imgFile.open(QIODevice::ReadOnly)) {
        QDataStream stream(&imgFile);
        int readed = stream.readRawData(imgData, sizeImageForRead);
        if (readed != -1) {
            QFile themeFile(filePath);
            if(themeFile.open(QIODevice::ReadWrite)) {
                sizeThemeForRead = themeFile.size();
                char *themeData = new char[sizeThemeForRead];
                QDataStream out_stream(&themeFile);
                int treaded = out_stream.readRawData(themeData, sizeThemeForRead);
                if (treaded != -1) {
                    out_stream.writeRawData(wps, 4);
                    out_stream.writeRawData(imgData, readed);
                    out_stream.writeRawData(wpe, 5);
                }
                delete themeData;
                themeFile.close();
            }
        }
    }
    imgFile.close();
    delete imgData;
}
