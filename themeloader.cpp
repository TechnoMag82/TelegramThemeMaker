#include "themeloader.h"

ThemeLoader::ThemeLoader()
{

}

void ThemeLoader::loadTheme(QString filePath, QList<ThemeItem *> &theme)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        qDeleteAll(theme);
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
            if (!line.isEmpty() && !line.startsWith("wallpaperFileOffset")) {
                int pos = line.indexOf('=');
                QString name = line.left(pos);
                QString color = line.right(line.size() - 1 - pos);
                ThemeItem *themeItem = new ThemeItem(name, color.toInt());
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
    if (file.open(QIODevice::WriteOnly)) {
        ThemeItem *item = nullptr;
        QTextStream stream(&file);
        int size = theme.count();
        for (int i = 0; i < size; i++) {
            item = theme.at(i);
            item->resetModify();
            stream << item->name << "=" << item->getRawColor() << "\n";
        }
        if (!imagePath.isNull() && !imagePath.isEmpty()) {
            stream.flush();
            QString streamPos = QString::number(stream.pos(), 10);
            QString wallpaperFileOffset = "wallpaperFileOffset=";
            quint64 pos = stream.pos() + wallpaperFileOffset.length() + streamPos.length() + 1;
            stream << wallpaperFileOffset << pos << "\n";
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

void ThemeLoader::deleteTempImageWallpaper(QString wallpaper)
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
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        int skipped = stream.skipRawData(startPos);
        if (skipped != -1) {
            int readed = stream.readRawData(data, sizeForRead);
            if (readed != -1) {
                mWallpaperPath = "wallpaper-" + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss") + ".jpg";
                QFile imgFile(mWallpaperPath);
                if (imgFile.open(QIODevice::WriteOnly)) {
                    QDataStream out_stream(&imgFile);
                    out_stream.writeRawData(data, readed);
                    imgFile.close();
                }
            }
        }
    }
    file.close();
    delete []data;
}

void ThemeLoader::saveImageTheme(QString filePath, QString imagePath)
{
    QFile imgFile(imagePath);
    int sizeImageForRead = imgFile.size();
    int sizeThemeForRead = imgFile.size();
    const char *wps = "WPS\n";
    const char *wpe = "\nWPE\n";
    char *imgData = new char[sizeImageForRead];
    if (imgFile.open(QIODevice::ReadOnly)) {
        QDataStream stream(&imgFile);
        int readed = stream.readRawData(imgData, sizeImageForRead);
        if (readed != -1) {
            QFile themeFile(filePath);
            if (themeFile.open(QIODevice::ReadWrite)) {
                sizeThemeForRead = themeFile.size();
                char *themeData = new char[sizeThemeForRead];
                QDataStream out_stream(&themeFile);
                int treaded = out_stream.readRawData(themeData, sizeThemeForRead);
                if (treaded != -1) {
                    out_stream.writeRawData(wps, 4);
                    out_stream.writeRawData(imgData, readed);
                    out_stream.writeRawData(wpe, 5);
                }
                delete []themeData;
                themeFile.close();
            }
        }
    }
    imgFile.close();
    delete []imgData;
}
