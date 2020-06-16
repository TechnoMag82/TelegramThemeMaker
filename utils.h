#include <QColor>

static QColor colorFromRaw(int color) {
    unsigned int alpha = (color & 0xff000000) >> 24;
    unsigned int red   = (color & 0x00ff0000) >> 16;
    unsigned int green = (color & 0x0000ff00) >> 8;
    unsigned int blue  = (color & 0x000000ff);
    return QColor(red, green, blue, alpha);
}

static int qColorToRaw(QColor color)
{
    return ((color.alpha() & 0xff) << 24)
            + ((color.red() & 0xff) << 16)
            + ((color.green() & 0xff) << 8)
            + (color.blue() & 0xff);
}
