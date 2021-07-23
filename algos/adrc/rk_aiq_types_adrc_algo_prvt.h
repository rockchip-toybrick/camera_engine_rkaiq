/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd . All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
#ifndef __RK_AIQ_TYPES_ADRC_ALGO_PRVT_H__
#define __RK_AIQ_TYPES_ADRC_ALGO_PRVT_H__

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ae/rk_aiq_types_ae_algo_int.h"
#include "af/rk_aiq_types_af_algo_int.h"
#include "rk_aiq_algo_types.h"
#include "xcam_log.h"
#include "rk_aiq_types_adrc_stat_v200.h"



#define LIMIT_VALUE(value,max_value,min_value)      (value > max_value? max_value : value < min_value ? min_value : value)
#define SHIFT6BIT(A)         (A*64)
#define SHIFT7BIT(A)         (A*128)
#define SHIFT10BIT(A)         (A*1024)
#define SHIFT11BIT(A)         (A*2048)
#define SHIFT12BIT(A)         (A*4096)

#define LIMIT_PARA(a,b,c,d,e)      (c+(a-e)*(b-c)/(d -e))

#define ENVLVMAX     (1.0)
#define ENVLVMIN     (0.0)
#define MOVECOEFMAX     (1.0)
#define MOVECOEFMIN     (0.0)
#define OEPDFMAX     (1.0)
#define OEPDFMIN     (0.0)
#define FOCUSLUMAMAX     (100)
#define FOCUSLUMAMIN     (1)
#define DAMPMAX     (1.0)
#define DAMPMIN     (0.0)
#define DARKPDFTHMAX     (1.0)
#define DARKPDFTHMIN     (0.0)
#define TOLERANCEMAX     (20.0)
#define TOLERANCEMIN     (0.0)
#define DARKPDFMAX     (1)
#define DARKPDFMIN     (0)
#define ISOMIN     (50)
#define ISOMAX     (204800)
#define DYNAMICRANGEMAX     (84)
#define DYNAMICRANGEMIN     (1)
#define ADRC_MAX_IQ_DOTS (13)




typedef enum AdrcState_e {
    ADRC_STATE_INVALID       = 0,
    ADRC_STATE_INITIALIZED   = 1,
    ADRC_STATE_STOPPED       = 2,
    ADRC_STATE_RUNNING       = 3,
    ADRC_STATE_LOCKED        = 4,
    ADRC_STATE_MAX
} AdrcState_t;

typedef struct DrcHandleData_s
{
    float sw_drc_offset_pow2;//
	float sw_drc_compres_scl;//sys
	float sw_drc_position;
	float sw_drc_delta_scalein;//
	float sw_drc_hpdetail_ratio;
	float sw_drc_lpdetail_ratio;
	float sw_drc_weicur_pix;//
	float sw_drc_weipre_frame;//
	float sw_drc_force_sgm_inv0;//
	float sw_drc_motion_scl;//
	float sw_drc_edge_scl;//
	float sw_drc_space_sgm_inv1;//
	float sw_drc_space_sgm_inv0;//
	float sw_drc_range_sgm_inv1;//
	float sw_drc_range_sgm_inv0;//
	float sw_drc_weig_maxl;//
	float sw_drc_weig_bilat;//
	float sw_drc_gain_y[ISP21_DRC_Y_NUM];
	float sw_drc_compres_y[ISP21_DRC_Y_NUM];//
	float sw_drc_scale_y[ISP21_DRC_Y_NUM];//
	float sw_drc_adrc_gain;
	float sw_drc_iir_weight;//
	float sw_drc_min_ogain;//

} DrcHandleData_t;

typedef struct AdrcPrevData_s
{
    float Ratio;
    float Envlv;
    float MoveCoef;
    float OEPdf;
    float TotalFocusLuma;
    float DarkPdf;
    float ISO;
    float DynamicRange;
    DrcHandleData_t DrcHandleData;
} AdrcPrevData_t;

typedef struct CurrAeResult_s {
    //TODO
    float MeanLuma[3];
    float LfrmDarkLuma;
    float LfrmDarkPdf;
    float LfrmOverExpPdf;
    float SfrmMaxLuma;
    float SfrmMaxLumaPdf;
    float GlobalEnvLv;
    float L2M_Ratio;
    float M2S_Ratio;
    float DynamicRange;
    float OEPdf; //the pdf of over exposure in long frame
    float DarkPdf; //the pdf of dark region in long frame
    float ISO; //use long frame

    float Lv_fac;
    float DarkPdf_fac;
    float Contrast_fac;
    float BlockLumaS[225];
    float BlockLumaM[25];
    float BlockLumaL[225];
} CurrAeResult_t;

typedef struct {
    unsigned char valid;
    int id;
    int depth;
} AfDepthInfo_t;

typedef struct CurrAfResult_s {
    unsigned int CurrAfTargetPos;
    unsigned int CurrAfTargetWidth;
    unsigned int CurrAfTargetHeight;
    AfDepthInfo_t AfDepthInfo[225];
    unsigned int GlobalSharpnessCompensated[225];
} CurrAfResult_t;

typedef struct CurrData_s
{
    float Ratio;
    float EnvLv;
    float MoveCoef;
    float DynamicRange;
    float OEPdf;
    float DarkPdf;
    float ISO;
    float DarkPdfTH;
    float TotalFocusLuma;
    float Damp;
    float LumaWeight[225];
    DrcHandleData_t DrcHandleData;
} CurrData_t;

typedef struct AdrcProcResData_s
{
    DrcProcRes_t DrcProcRes;
    bool LongFrameMode;
    bool isHdrGlobalTmo;
    bool bTmoEn;
    bool isLinearTmo;
} AdrcProcResData_t;

typedef struct SensorInfo_s
{
    bool  LongFrmMode;
    float HdrMinGain[MAX_HDR_FRAMENUM];
    float HdrMaxGain[MAX_HDR_FRAMENUM];
    float HdrMinIntegrationTime[MAX_HDR_FRAMENUM];
    float HdrMaxIntegrationTime[MAX_HDR_FRAMENUM];

    float MaxExpoL;
    float MinExpoL;
    float MaxExpoM;
    float MinExpoM;
    float MaxExpoS;
    float MinExpoS;
} SensorInfo_t;

typedef struct AdrcContext_s
{
    //api
    drcAttr_t drcAttr;

    CalibDb_Adrc_Para_t pCalibDB;
    AdrcState_t state;
    AdrcConfig_t AdrcConfig;
    AdrcPrevData_t AdrcPrevData ;
    AdrcProcResData_t AdrcProcRes;
    CurrAeResult_t CurrAeResult;
    CurrAfResult_t CurrAfResult;
    CurrData_t CurrHandleData;
    rkisp_adrc_stats_t CurrStatsData;
    SensorInfo_t SensorInfo;
    uint32_t width;
    uint32_t height;
    int frameCnt;
    int FrameNumber;
    int sence_mode;
} AdrcContext_t;

typedef AdrcContext_t* AdrcHandle_t;

typedef struct AdrcInstanceConfig_s {
    AdrcHandle_t              hAdrc;
} AdrcInstanceConfig_t;

typedef struct _RkAiqAlgoContext {
    AdrcInstanceConfig_t AdrcInstConfig;
    //void* place_holder[0];
} RkAiqAlgoContext;

#endif
