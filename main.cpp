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
    //ImageHandler::gaussianSmoothing(image);
    ImageHandler::histogramEqualization(image);
    image.save("C:\\Users\\AC_Phoenix\\Desktop\\arborgreens\\medianFilter_equalization_Res.jpg");

/*
    QColor colorRgb(10, 10, 10);
    QColor colorHsl = colorRgb.toHsl();
    std::cout << "Hue = " << colorHsl.hslHue()
              << " Saturation = " << colorHsl.hslSaturation()
              << " Lightness = " << colorHsl.lightness() << std::endl << std::endl;

    colorRgb = colorHsl.toRgb();
    std::cout << "Red = " << colorHsl.red()
              << " Green = " << colorHsl.green()
              << " Blue = " << colorHsl.blue() << std::endl;
*/
    return a.exec();
}
