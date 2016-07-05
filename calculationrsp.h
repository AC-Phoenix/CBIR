#ifndef CALCULATIONRSP
#define CALCULATIONRSP

struct ColorFeatureRsp
{
    double HM1;
    double HM2;
    double HM3;
    double SM1;
    double SM2;
    double SM3;
    double LM1;
    double LM2;
    double LM3;
};

struct ShapeFeatureRsp
{
    // 熵
    double entropyE;
    double entropySD;
    // 能量
    double energyE;
    double energySD;
    // 对比度
    double contrastE;
    double contrastSD;
    // 逆差矩
    double IDME;
    double IDMSD;
    // 自相关性
    double correlationE;
    double correlationSD;
};

#endif // CALCULATIONRSP

