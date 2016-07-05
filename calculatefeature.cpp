#include "calculatefeature.h"
#include <iostream>
#include <cstdio>
#include <QImage>
#include <QColor>
#include <QSize>
#include <cmath>
#include "imagehandler.h"
#include "glcm.h"

CalculateFeature::CalculateFeature()
{

}

CalculateFeature::~CalculateFeature()
{

}

void CalculateFeature::calColorFeature(const char *path, ColorFeatureRsp &rsp)
{
    QImage image;
    image.load(path);
    ImageHandler::scaled(image);
    ImageHandler::medianFilter(image);
    ImageHandler::histogramEqualization(image);
    //ImageHandler::toGray(image);
    int height = image.height();
    int width  = image.width();
    int N = height*width;
    rsp.HM1 = 0;
    rsp.SM1 = 0;
    rsp.LM1 = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor colorHsl = image.pixel(x, y);
            rsp.HM1 += colorHsl.hslHueF();
            rsp.SM1 += colorHsl.hslSaturationF();
            rsp.LM1 += colorHsl.lightnessF();
        }
    }
    rsp.HM1 /= N;
    rsp.SM1 /= N;
    rsp.LM1 /= N;

    rsp.HM2 = 0;    rsp.HM3 = 0;
    rsp.SM2 = 0;    rsp.SM3 = 0;
    rsp.LM2 = 0;    rsp.LM3 = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor colorHsl = image.pixel(x, y);
            double H = colorHsl.hslHueF();
            double S = colorHsl.hslSaturationF();
            double L = colorHsl.lightnessF();

            rsp.HM2 += (H-rsp.HM1) * (H-rsp.HM1);    rsp.HM3 += (H-rsp.HM1) * (H-rsp.HM1) * (H-rsp.HM1);
            rsp.SM2 += (S-rsp.SM1) * (S-rsp.SM1);    rsp.SM3 += (S-rsp.SM1) * (S-rsp.SM1) * (S-rsp.SM1);
            rsp.LM2 += (L-rsp.LM1) * (L-rsp.LM1);    rsp.LM3 += (L-rsp.LM1) * (L-rsp.LM1) * (L-rsp.LM1);
        }
    }
    rsp.HM2 /= N;   rsp.HM3 /= N;
    rsp.SM2 /= N;   rsp.SM3 /= N;
    rsp.LM2 /= N;   rsp.LM3 /= N;

    int Hsign = (rsp.HM3 < 0) ? -1 : 1;
    int Ssign = (rsp.SM3 < 0) ? -1 : 1;
    int Lsign = (rsp.LM3 < 0) ? -1 : 1;

    rsp.HM2 = sqrt(rsp.HM2);   rsp.HM3 = pow(rsp.HM3*Hsign, 1.0/3);
    rsp.SM2 = sqrt(rsp.SM2);   rsp.SM3 = pow(rsp.SM3*Ssign, 1.0/3);
    rsp.LM2 = sqrt(rsp.LM2);   rsp.LM3 = pow(rsp.LM3*Lsign, 1.0/3);

    rsp.HM3 *= Hsign;
    rsp.SM3 *= Ssign;
    rsp.LM3 *= Lsign;
}

void CalculateFeature::calShapeFeature(const char *path, ShapeFeatureRsp &rsp)
{
    QImage image;
    image.load(path);
    ImageHandler::scaled(image);
    ImageHandler::medianFilter(image);
    ImageHandler::histogramEqualization(image);
    ImageHandler::toGray(image);

    int dx[] = {1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1};
    const int N = 4;

    // 熵
    rsp.entropyE  = 0;
    rsp.entropySD = 0;
    // 能量
    rsp.energyE  = 0;
    rsp.energySD = 0;
    // 对比度
    rsp.contrastE  = 0;
    rsp.contrastSD = 0;
    // 逆差矩
    rsp.IDME  = 0;
    rsp.IDMSD = 0;
    // 自相关性
    rsp.correlationE  = 0;
    rsp.correlationSD = 0;

    GLCM glcm[N];
    GLCMFeature res[N];
    for (int i = 0; i < N; ++i)
    {
        glcm[i].readImage(image, dx[i], dy[i]);
        glcm[i].calFeature(res[i]);
        rsp.entropyE     += res[i].entropy;
        rsp.energyE      += res[i].energy;
        rsp.contrastE    += res[i].contrast;
        rsp.IDME         += res[i].IDM;
        rsp.correlationE += res[i].correlation;
    }
    rsp.entropyE     /= N;
    rsp.energyE      /= N;
    rsp.contrastE    /= N;
    rsp.IDME         /= N;
    rsp.correlationE /= N;

    for (int i = 0; i < N; ++i)
    {
        rsp.entropySD     += (res[i].entropy-rsp.entropyE) * (res[i].entropy-rsp.entropyE);
        rsp.energySD      += (res[i].energy-rsp.energyE) * (res[i].energy-rsp.energyE);
        rsp.contrastSD    += (res[i].contrast-rsp.contrastE) * (res[i].contrast-rsp.contrastE);
        rsp.IDMSD         += (res[i].IDM-rsp.IDME) * (res[i].IDM-rsp.IDME);
        rsp.correlationSD += (res[i].correlation-rsp.correlationE) * (res[i].correlation-rsp.correlationE);
    }
    rsp.entropySD     = sqrt(rsp.entropySD/N);
    rsp.energySD      = sqrt(rsp.energySD/N);
    rsp.contrastSD    = sqrt(rsp.contrastSD/N);
    rsp.IDMSD         = sqrt(rsp.IDMSD/N);
    rsp.correlationSD = sqrt(rsp.correlationSD/N);

}
