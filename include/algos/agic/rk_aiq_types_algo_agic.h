
#ifndef __RKAIQ_TYPES_ALGO_AGIC_H__
#define __RKAIQ_TYPES_ALGO_AGIC_H__

typedef struct AgicProcResultV21_s {
    int regmingradthrdark2;
    int regmingradthrdark1;
    int regminbusythre;
    int regdarkthre;
    int regmaxcorvboth;
    int regdarktthrehi;
    int regkgrad2dark;
    int regkgrad1dark;
    int regstrengthglobal_fix;
    int regdarkthrestep;
    int regkgrad2;
    int regkgrad1;
    int reggbthre;
    int regmaxcorv;
    int regmingradthr2;
    int regmingradthr1;
    int gr_ratio;
    int noise_scale;
    int noise_base;
    int diff_clip;
    int sigma_y[15];
} AgicProcResultV21_t;

typedef struct AgicProcResultV20_s {
    int edge_open;
    int regmingradthrdark2;
    int regmingradthrdark1;
    int regminbusythre;
    int regdarkthre;
    int regmaxcorvboth;
    int regdarktthrehi;
    int regkgrad2dark;
    int regkgrad1dark;
    int regstrengthglobal_fix;
    int regdarkthrestep;
    int regkgrad2;
    int regkgrad1;
    int reggbthre;
    int regmaxcorv;
    int regmingradthr2;
    int regmingradthr1;
    int gr_ratio;
    int dnloscale;
    int dnhiscale;
    int reglumapointsstep;
    int gvaluelimitlo;
    int gvaluelimithi;
    int fusionratiohilimt1;
    int regstrength_fix;
    int sigma_y[15];
    int noise_cut_en;
    int noise_coe_a;
    int noise_coe_b;
    int diff_clip;
} AgicProcResultV20_t;

typedef struct AgicProcResult_s {
    union {
        AgicProcResultV20_t ProcResV20;
        AgicProcResultV21_t ProcResV21;
    };
    bool gic_en;
} AgicProcResult_t;

typedef struct AgicConfigV20_s {
    unsigned char gic_en;
    unsigned char edge_open;
    unsigned short regmingradthrdark2;
    unsigned short regmingradthrdark1;
    unsigned short regminbusythre;
    unsigned short regdarkthre;
    unsigned short regmaxcorvboth;
    unsigned short regdarktthrehi;
    unsigned char regkgrad2dark;
    unsigned char regkgrad1dark;
    float globalStrength;
    unsigned char regkgrad2;
    unsigned char regkgrad1;
    unsigned char reggbthre;
    unsigned short regmaxcorv;
    unsigned short regmingradthr2;
    unsigned short regmingradthr1;
    unsigned char gr_ratio;
    float dnloscale;
    float dnhiscale;
    unsigned char reglumapointsstep;
    float gvaluelimitlo;
    float gvaluelimithi;
    float fusionratiohilimt1;
    float textureStrength;
    float sigma_y[15];
    unsigned char noise_cut_en;
    unsigned short noise_coe_a;
    unsigned short noise_coe_b;
    unsigned short diff_clip;
} AgicConfigV20_t;

typedef struct AgicConfigV21_s {
    unsigned char gic_en;
    unsigned short regmingradthrdark2;
    unsigned short regmingradthrdark1;
    unsigned short regminbusythre;
    unsigned short regdarkthre;
    unsigned short regmaxcorvboth;
    unsigned short regdarktthrehi;
    unsigned char regkgrad2dark;
    unsigned char regkgrad1dark;
    unsigned char regkgrad2;
    unsigned char regkgrad1;
    unsigned char reggbthre;
    unsigned short regmaxcorv;
    unsigned short regmingradthr2;
    unsigned short regmingradthr1;
    unsigned char gr_ratio;
    float noise_scale;
    float noise_base;
    float sigma_y[15];
    float globalStrength;
    unsigned short diff_clip;
} AgicConfigV21_t;

typedef struct AgicConfig_s {
    union {
        AgicConfigV20_t ConfigV20;
        AgicConfigV21_t ConfigV21;
    };
} AgicConfig_t;


#endif//__RKAIQ_TYPES_ALGO_AGIC_H__
