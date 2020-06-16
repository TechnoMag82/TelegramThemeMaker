#ifndef THEMEITEM_H
#define THEMEITEM_H

#include <QColor>
#include <QString>

class ThemeItem
{
private:
    int colorRaw;
    QColor colorValue;
    int oldColorRaw;
    QColor oldColorValue;
    bool modified;
    void mSetRawColor(int color);

public:
    ThemeItem();
    ThemeItem(QString name, int color);
    QString name;
    void setRawColor(int color);
    QColor getColor();
    int getRawColor();
    bool isModified();
    void resetToDefault();
    void resetModify();
};

#endif // THEMEITEM_H
