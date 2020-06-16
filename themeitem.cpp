#include "themeitem.h"
#include "utils.h"

ThemeItem::ThemeItem()
{
    modified = false;
}

ThemeItem::ThemeItem(QString name, int color)
{
    this->name = name;
    modified = false;
    mSetRawColor(color);
}

void ThemeItem::mSetRawColor(int color)
{
    oldColorRaw = colorRaw;
    oldColorValue = colorValue;
    colorRaw = color;
    colorValue = colorFromRaw(color);
}

void ThemeItem::setRawColor(int color)
{
    modified = true;
    mSetRawColor(color);
}

QColor ThemeItem::getColor()
{
    return colorValue;
}

int ThemeItem::getRawColor()
{
    return colorRaw;
}

bool ThemeItem::isModified()
{
    return modified;
}

void ThemeItem::resetToDefault()
{
    colorRaw = oldColorRaw;
    colorValue = oldColorValue;
    modified = false;
}

void ThemeItem::resetModify()
{
    modified = false;
}
