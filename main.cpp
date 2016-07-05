#include <QCoreApplication>
#include <iostream>
#include <cstdio>
#include <QImage>
#include <QColor>
#include <QSize>
#include "imagehandler.h"
#include "calculatefeature.h"

char prefix[] = "C:\\Users\\AC_Phoenix\\Desktop\\arborgreens\\";
char path[100];

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ImageHandler::init();
    for (int i = 1; i <= 47; ++i)
    {
        printf("Image%02d.jpg:\n", i);
        sprintf(path, "%sImage%02d.jpg", prefix, i);
        ShapeFeatureRsp rsp;
        CalculateFeature::calShapeFeature(path, rsp);

        std::cout << "rsp.entropyE = "  << rsp.entropyE  << std::endl;
        std::cout << "rsp.entropySD = " << rsp.entropySD << std::endl;
        std::cout << "rsp.energyE = "  << rsp.energyE  << std::endl;
        std::cout << "rsp.energySD = " << rsp.energySD << std::endl;
        std::cout << "rsp.contrastE =  " << rsp.contrastE  << std::endl;
        std::cout << "rsp.contrastSD = " << rsp.contrastSD << std::endl;
        std::cout << "rsp.IDME =  " << rsp.IDME  << std::endl;
        std::cout << "rsp.IDMSD = " << rsp.IDMSD << std::endl;
        std::cout << "rsp.correlationE =  " << rsp.correlationE  << std::endl;
        std::cout << "rsp.correlationSD = " << rsp.correlationSD << std::endl << std::endl;

    }


    /* calColorFeature
    ColorFeatureRsp rsp;
    CalculateFeature::calColorFeature("C:\\Users\\AC_Phoenix\\Desktop\\arborgreens\\Image01.jpg", rsp);

    std::cout << "rsp.HM1 = " << rsp.HM1 << std::endl;
    std::cout << "rsp.HM2 = " << rsp.HM2 << std::endl;
    std::cout << "rsp.HM3 = " << rsp.HM3 << std::endl;
    std::cout << "rsp.SM1 = " << rsp.SM1 << std::endl;
    std::cout << "rsp.SM2 = " << rsp.SM2 << std::endl;
    std::cout << "rsp.SM3 = " << rsp.SM3 << std::endl;
    std::cout << "rsp.LM1 = " << rsp.LM1 << std::endl;
    std::cout << "rsp.LM2 = " << rsp.LM2 << std::endl;
    std::cout << "rsp.LM3 = " << rsp.LM3 << std::endl;
    */
/*
    std::cout << "begin" << std::endl;
    QImage image;
    sprintf(path, "%s%s", prefix, "Image16.jpg");
    image.load(path);

    ImageHandler::scaled(image);
    sprintf(path, "%s%s", prefix, "Image16_1_scaled.jpg");
    image.save(path);

    ImageHandler::medianFilter(image);
    sprintf(path, "%s%s", prefix, "Image16_2_medianFilter.jpg");
    image.save(path);
    //ImageHandler::gaussianSmoothing(image);

    ImageHandler::histogramEqualization(image);
    sprintf(path, "%s%s", prefix, "Image16_3_histogramEqualization.jpg");
    image.save(path);

    ImageHandler::toGray(image);
    sprintf(path, "%s%s", prefix, "Image16_4_toGray.bmp");
    image.save(path);
    std::cout << "allGray() = " << image.allGray() << std::endl;
    std::cout << "isGrayscale() = " << image.isGrayscale() << std::endl;
*/

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
    std::cout << "end" << std::endl;
    return a.exec();
}
