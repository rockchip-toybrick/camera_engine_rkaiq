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
#ifndef __RK_AIQ_TYPES_ADRC_ALGO_INT_H__
#define __RK_AIQ_TYPES_ADRC_ALGO_INT_H__

#include "RkAiqCalibDbTypes.h"
#include "rk_aiq_types_adrc_stat_v200.h"


enum {
    ADRC_NORMAL = 0,
    ADRC_HDR = 1,
    ADRC_NIGHT = 2
};

typedef enum drc_OpMode_s {
    DRC_OPMODE_API_OFF = 0, // run IQ ahdr
    DRC_OPMODE_AUTO = 1, //run api auto ahdr
    DRC_OPMODE_MANU= 2, //run api manual ahdr
    DRC_OPMODE_SET_LEVEL = 3, // its prevously fast mode, run api set level
    DRC_OPMODE_DARKAREA = 4, // for dark area luma inprove, no matter the scene is night, normal, or hdr
    DRC_OPMODE_TOOL = 5, // for dark area luma inprove, no matter the scene is night, normal, or hdr
} drc_OpMode_t;

typedef CalibDb_Drc_ModeCell_t AdrcConfig_t;

typedef struct adrcAttr_s
{
    
} adrcAttr_t;

typedef struct mdrcAttr_s
{
    
} mdrcAttr_t;

typedef struct drcAttr_s
{
    drc_OpMode_t    opMode;
    adrcAttr_t    stAuto;
    mdrcAttr_t stManual;
    //CurrCtlData_t CtlInfo;
    //CurrRegData_t RegInfo;
    CalibDb_Adrc_Para_t stTool;
} drcAttr_t;

typedef struct DrcProcRes_s
{
	int sw_drc_offset_pow2;//
	int sw_drc_compres_scl;//sys
	int sw_drc_position;
	int sw_drc_delta_scalein;//
	int sw_drc_hpdetail_ratio;
	int sw_drc_lpdetail_ratio;
	int sw_drc_weicur_pix;//
	int sw_drc_weipre_frame;//
	int sw_drc_force_sgm_inv0;//
	int sw_drc_motion_scl;//
	int sw_drc_edge_scl;//
	int sw_drc_space_sgm_inv1;//
	int sw_drc_space_sgm_inv0;//
	int sw_drc_range_sgm_inv1;//
	int sw_drc_range_sgm_inv0;//
	int sw_drc_weig_maxl;//
	int sw_drc_weig_bilat;//
	int sw_drc_gain_y[ISP21_DRC_Y_NUM];
	int sw_drc_compres_y[ISP21_DRC_Y_NUM];//
	int sw_drc_scale_y[ISP21_DRC_Y_NUM];//
	int sw_drc_adrc_gain;
	int sw_drc_iir_weight;//
	int sw_drc_min_ogain;//

} DrcProcRes_t;

typedef struct RkAiqAdrcProcResult_s
{
    DrcProcRes_t DrcProcRes;
    drcAttr_t drcAttr;
    bool LongFrameMode;
    bool isHdrGlobalTmo;
    bool bTmoEn;
    bool isLinearTmo;
} RkAiqAdrcProcResult_t;


#endif
