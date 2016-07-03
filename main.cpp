#include <QCoreApplication>
#include <iostream>
#include <cstdio>
#include <QImage>
#include <QColor>
#include <QSize>
#include "imagehandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ImageHandler::init();

    std::cout << "asdf" << std::endl;
    QImage image;

    image.load("C:\\Users\\AC_Phoenix\\Desktop\\arborgreens\\medianFilterSrc.jpg");
    ImageHandler::scaled(image);
    ImageHandler::medianFilter(image);
    image.save("C:\\Users\\AC_Phoenix\\Desktop\\arborgreens\\medianFilterRes.jpg");

/*
    for (int y = 0; y < image3.height(); ++y)
    {
        for (int x = 0; x < image3.width(); ++x)
        {
            QRgb rgb = image1.pixel(x, y);
            image3.setPixel(x, y, rgb);
        }
    }
*/

    return a.exec();
}
