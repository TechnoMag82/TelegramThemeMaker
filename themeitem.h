#ifndef THEMEITEM_H
#define THEMEITEM_H

#include <QColor>
#include <QString>

class ThemeItem
{
private:
    int colorRaw;
    QColor colorValue;
public:
    ThemeItem();
    QString name;
    void setRawColor(int color);
    QColor getColor();
    int getRawColor();
};

#endif // THEMEITEM_H
