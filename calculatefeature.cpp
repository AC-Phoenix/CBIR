#include "calculatefeature.h"
#include <iostream>
#include <cstdio>
#include <QImage>
#include <QColor>
#include <QSize>
#include <cmath>
#include "imagehandler.h"
#include "glcm.h"

int robertsH1[3][3] = {0, -1, 0, 1, 0, 0, 0, 0, 0};
int robertsH2[3][3] = {-1, 0, 0, 0, 1, 0, 0, 0, 0};
int laplacianA[3][3] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
int laplacianB[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
int laplacianC[3][3] = {1, -2, 1, -2, 4, -2, 1, -2, 1};

int sobelX[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int sobelY[3][3] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

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

void CalculateFeature::calTextureFeature(const char *path, TextureFeatureRsp &rsp)
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

void CalculateFeature::calShapeOneDirection(const QImage &image, ShapeFeature &ret)
{
    for (int i = 0; i < 7; ++i)
    {
        ret.M[i] = 0;
    }
    ret.Eccentricity = 0;
    int height = image.height();
    int width  = image.width();
    // 中心矩
    double m00 = 0;
    double m11 = 0;
    double m20 = 0;
    double m02 = 0;
    double m30 = 0;
    double m03 = 0;
    double m12 = 0;
    double m21 = 0;

    // 规范化后的中心矩
    double u20 = 0;
    double u02 = 0;
    double u11 = 0;
    double u30 = 0;
    double u03 = 0;
    double u12 = 0;
    double u21 = 0;

    double t1 = 0;
    double t2 = 0;
    double t3 = 0;
    double t4 = 0;
    double t5 = 0;
    // 重心
    int centerX = 0;
    int centerY = 0;
    //0阶矩和1阶矩
    double s10 = 0;
    double s01 = 0;
    double s00 = 0;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QColor color = image.pixel(x, y);
            int grayLevel = color.red();
            s10 += x*grayLevel;
            s01 += y*grayLevel;
            s00 += grayLevel;
        }
    }
    centerX = (int)(s10/s00 + 0.5);
    centerY = (int)(s01/s00 + 0.5);
    /*
    std::cout << "centerX = " << centerX << std::endl;
    std::cout << "centerY = " << centerY << std::endl;
    */
    //  计算二阶、三阶矩(中心矩)
    m00 = s00;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double x0 = x-centerX;
            double y0 = y-centerY;
            /*
            std::cout << "x0 = " << x0 << std::endl;
            std::cout << "y0 = " << y0 << std::endl;
            */
            QColor color = image.pixel(x, y);
            int grayLevel = color.red();
            m11 += x0*y0*grayLevel;
            m20 += x0*x0*grayLevel;
            m02 += y0*y0*grayLevel;
            m03 += y0*y0*y0*grayLevel;
            m30 += x0*x0*x0*grayLevel;
            m12 += x0*y0*y0*grayLevel;
            m21 += x0*x0*y0*grayLevel;
            /*
            std::cout << "m11 = " << m11 << std::endl;
            std::cout << "m20 = " << m20 << std::endl;
            std::cout << "m02 = " << m02 << std::endl;
            std::cout << "m03 = " << m03 << std::endl;
            std::cout << "m30 = " << m30 << std::endl;
            std::cout << "m12 = " << m12 << std::endl;
            std::cout << "m21 = " << m21 << std::endl;
            */
        }
    }
    /*
    std::cout << "m11 = " << m11 << std::endl;
    std::cout << "m20 = " << m20 << std::endl;
    std::cout << "m02 = " << m02 << std::endl;
    std::cout << "m03 = " << m03 << std::endl;
    std::cout << "m30 = " << m30 << std::endl;
    std::cout << "m12 = " << m12 << std::endl;
    std::cout << "m21 = " << m21 << std::endl;
    */
    //  计算规范化后的中心矩: mij/pow(m00, ((i+j+2)/2)
    u20 = m20/(m00*m00);
    u02 = m02/(m00*m00);
    u11 = m11/(m00*m00);
    u30 = m30/pow(m00, 2.5);
    u03 = m03/pow(m00, 2.5);
    u12 = m12/pow(m00, 2.5);
    u21 = m21/pow(m00, 2.5);
    /*
    std::cout << "u20 = " << u20 << std::endl;
    std::cout << "u02 = " << u02 << std::endl;
    std::cout << "u11 = " << u11 << std::endl;
    std::cout << "u30 = " << u30 << std::endl;
    std::cout << "u03 = " << u03 << std::endl;
    std::cout << "u12 = " << u12 << std::endl;
    std::cout << "u21 = " << u21 << std::endl;
    */
    //  计算中间变量
    t1 = u20 - u02;
    t2 = u30 - 3*u12;
    t3 = 3*u21 - u03;
    t4 = u30 + u12;
    t5 = u21 + u03;
/*
    std::cout << "t1 = " << t1 << std::endl;
    std::cout << "t2 = " << t2 << std::endl;
    std::cout << "t3 = " << t3 << std::endl;
    std::cout << "t4 = " << t4 << std::endl;
    std::cout << "t5 = " << t5 << std::endl;
*/
    // 计算不变矩
    ret.M[0] = u20 + u02;
    ret.M[1] = t1*t1 + 4*u11*u11;
    ret.M[2] = t2*t2 + t3*t3;
    ret.M[3] = t4*t4 + t5*t5;
    ret.M[4] = t2*t4*(t4*t4 - 3*t5*t5) + t3*t5*(3*t4*t4 - t5*t5);
    ret.M[5] = t1*(t4*t4 - t5*t5) + 4*u11*t4*t5;
    ret.M[6] = t3*t4*(t4*t4 - 3*t5*t5) - t2*t5*(3*t4*t4 - t5*t5);
}

void CalculateFeature::calShapeFeature(const char *path, ShapeFeatureRsp &rsp)
{
    QImage imageX, imageY;
    imageX.load(path);
    ImageHandler::scaled(imageX);
    ImageHandler::medianFilter(imageX);
    ImageHandler::toGray(imageX);
    imageY = imageX;

    ImageHandler::sharpening(imageX, sobelX, 3);
    ImageHandler::sharpening(imageY, sobelY, 3);
    ImageHandler::otsu(imageX);
    ImageHandler::otsu(imageY);
    calShapeOneDirection(imageX, rsp.Xdirection);
    calShapeOneDirection(imageY, rsp.Ydirection);
}
