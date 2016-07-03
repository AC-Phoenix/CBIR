#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QImage>
#include <QSize>
#include <QRgb>
#include <QColor>

class ImageHandler
{
public:
    ImageHandler();
    ~ImageHandler();

    static void init()
    {
        // size的值要改为在文件中读取
        size.scale(800, 600, Qt::IgnoreAspectRatio);
    }

    static void scaled(QImage &image)
    {
        image = image.scaled(size);
    }

    static void medianFilter(QImage &image, int D = 3);

private:
    static QSize size;
};


#endif // IMAGEHANDLER_H
