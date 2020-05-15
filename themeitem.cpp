#include "themeitem.h"

ThemeItem::ThemeItem()
{

}

void ThemeItem::setRawColor(int color)
{
    colorRaw = color;
    unsigned int alpha = (color & 0xff000000) >> 24;
    unsigned int red   = (color & 0x00ff0000) >> 16;
    unsigned int green = (color & 0x0000ff00) >> 8;
    unsigned int blue  = (color & 0x000000ff);
    colorValue = QColor(red, green, blue, alpha);
}

QColor ThemeItem::getColor()
{
    return colorValue;
}

int ThemeItem::getRawColor()
{
    return colorRaw;
}
