/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
/**
 * @file adrc.cpp
 *
 * @brief
 *   ADD_DESCRIPTION_HERE
 *
 *****************************************************************************/
#include "math.h"
#include "rk_aiq_types_adrc_algo_int.h"
#include "xcam_log.h"
#include "rk_aiq_types_adrc_algo_prvt.h"


/******************************************************************************
 * AdrcStart()
 *****************************************************************************/
XCamReturn AdrcStart
(
    AdrcHandle_t pAdrcCtx
) {

    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    // initial checks
    if (pAdrcCtx == NULL) {
        return (XCAM_RETURN_ERROR_MEM);
    }

    if ((ADRC_STATE_RUNNING == pAdrcCtx->state)
            || (ADRC_STATE_LOCKED == pAdrcCtx->state)) {
        return (XCAM_RETURN_ERROR_PARAM);
    }

    pAdrcCtx->state = ADRC_STATE_RUNNING;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
    return (XCAM_RETURN_NO_ERROR);
}
/******************************************************************************
 * AdrcStop()
 *****************************************************************************/
XCamReturn AdrcStop
(
    AdrcHandle_t pAdrcCtx
) {

    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    // initial checks
    if (pAdrcCtx == NULL) {
        return (XCAM_RETURN_ERROR_MEM);
    }

    // before stopping, unlock the AHDR if locked
    if (ADRC_STATE_LOCKED == pAdrcCtx->state) {
        return (XCAM_RETURN_ERROR_PARAM);
    }

    pAdrcCtx->state = ADRC_STATE_STOPPED;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);

    return (XCAM_RETURN_NO_ERROR);
}
/******************************************************************************
 * AdrcGetCurrPara()
 *****************************************************************************/
float AdrcAdrcGetCurrPara
(
    float           inPara,
    float         inMatrixX[ADRC_MAX_IQ_DOTS],
    float         inMatrixY[ADRC_MAX_IQ_DOTS]
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);
    float x1 = 0.0f;
    float x2 = 0.0f;
    float value1 = 0.0f;
    float value2 = 0.0f;
    float outPara = 0.0f;

    if(inPara < inMatrixX[0])
        outPara = inMatrixY[0];
    else if (inPara >= inMatrixX[ADRC_MAX_IQ_DOTS - 1])
        outPara = inMatrixY[ADRC_MAX_IQ_DOTS - 1];
    else
        for(int i = 0; i < ADRC_MAX_IQ_DOTS - 2; i++)
        {
            if(inPara >= inMatrixX[i] && inPara < inMatrixX[i + 1])
            {
                x1 = inMatrixX[i];
                x2 = inMatrixX[i + 1];
                value1 = inMatrixY[i];
                value2 = inMatrixY[i + 1];
                outPara = value1 + (inPara - x1) * (value1 - value2) / (x1 - x2);
                break;
            }
            else
                continue;
        }

    return outPara;
    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
* CalibrateDrcGainY()
*****************************************************************************/
void CalibrateDrcGainY( DrcProcRes_t *para)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);

    float detailslowlight = 0.2;
    float tmp = 0;

    float luma[17] = { 0, 256, 512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560, 2816, 3072, 3328, 3584, 3840, 4096 };
    float gainTable[17];
    float maxgain = (float)(para->sw_drc_adrc_gain);

    for(int i = 0; i < 17; ++i)
    {

        tmp = luma[i];
        tmp = 1 - detailslowlight * pow((1 - tmp / 4096), 2);
        gainTable[i] = 1024 * pow(maxgain / 1024, tmp);
        para->sw_drc_gain_y[i] = (int)(gainTable[i]) ;
    }


    LOG1_AHDR("%s:Eixt!\n", __FUNCTION__);

}

/******************************************************************************
 * AdrcApiOffConfig()
 *set default AdrcConfig data
 *****************************************************************************/
void AdrcApiOffConfig
(
    AdrcHandle_t           pAdrcCtx
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    pAdrcCtx->AdrcConfig.sw_drc_offset_pow2 = 8;
    pAdrcCtx->AdrcConfig.sw_drc_position = 4096 ;//
    pAdrcCtx->AdrcConfig.sw_drc_hpdetail_ratio = 0 ;
    pAdrcCtx->AdrcConfig.sw_drc_lpdetail_ratio = 0 ;
    pAdrcCtx->AdrcConfig.sw_drc_weicur_pix = 96 ;
    pAdrcCtx->AdrcConfig.sw_drc_weipre_frame = 255;
    pAdrcCtx->AdrcConfig.sw_drc_force_sgm_inv0 = 0 ;
    pAdrcCtx->AdrcConfig.sw_drc_motion_scl = 0 ;
    pAdrcCtx->AdrcConfig.sw_drc_edge_scl = 16 ;
    pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv1 = 4068 ;
    pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv0 = 3968 ;
    pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv1 = 1024 ;
    pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv0 = 1024;
    pAdrcCtx->AdrcConfig.sw_drc_weig_maxl = 0 ;
    pAdrcCtx->AdrcConfig.sw_drc_weig_bilat = 16 ;
    pAdrcCtx->AdrcConfig.sw_drc_iir_frame = 16 ;
    pAdrcCtx->AdrcConfig.sw_drc_gain = 1024 ;
    pAdrcCtx->AdrcConfig.sw_drc_min_ogain = 0 ;
    for(int i = 0; i < ISP21_DRC_Y_NUM; ++i)
        pAdrcCtx->AdrcConfig.sw_drc_scale_y[i] = 80 ;


    LOGI_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcConfig()
 *set default AdrcConfig data
 *****************************************************************************/
void AdrcConfig
(
    AdrcHandle_t           pAdrcCtx
) {
    LOGI_AHDR( "%s:enter!\n", __FUNCTION__);

    // initial checks
    DCT_ASSERT(pAdrcCtx != NULL);

    if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_API_OFF)
    {
        AdrcApiOffConfig(pAdrcCtx);
        LOGD_AHDR("%s: Ahdr Api is OFF!!!:\n", __FUNCTION__);
    }
    else
        LOGD_AHDR("%s: Ahdr Api is ON!!!:\n", __FUNCTION__);

    //config default AdrcPrevData data
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_offset_pow2 = 8;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_compres_scl = 20000;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_position = 0.005 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_delta_scalein = 0.01 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_hpdetail_ratio = 0 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_lpdetail_ratio = 0 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_weicur_pix = 96 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_weipre_frame = 255;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_force_sgm_inv0 = 0 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_motion_scl = 0 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_edge_scl = 16 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_space_sgm_inv1 = 4068 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_space_sgm_inv0 = 3968 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_range_sgm_inv1 = 1024 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_range_sgm_inv0 = 1024;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_weig_maxl = 0 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_weig_bilat = 16 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_iir_weight = 60 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_adrc_gain = 1024 ;
    pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_min_ogain = 0 ;
    for(int i = 0; i < ISP21_DRC_Y_NUM; ++i)
    {
        pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_gain_y[i] = 80 ;
        pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_compres_y[i] = 205 ;
        pAdrcCtx->AdrcPrevData.DrcHandleData.sw_drc_scale_y[i] = 80 ;
    }

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}
void AdrcGetStats
(
    AdrcHandle_t           pAdrcCtx,
    rkisp_adrc_stats_t*         ROData
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);




    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}
void AdrcGetAeResult
(
    AdrcHandle_t           pAdrcCtx,
    AecPreResult_t  AecHdrPreResult
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    //get Ae Pre Result
    pAdrcCtx->CurrAeResult.GlobalEnvLv = AecHdrPreResult.GlobalEnvLv[AecHdrPreResult.NormalIndex];
    pAdrcCtx->CurrAeResult.M2S_Ratio = AecHdrPreResult.M2S_ExpRatio;
    pAdrcCtx->CurrAeResult.M2S_Ratio = pAdrcCtx->CurrAeResult.M2S_Ratio < 1 ? 1 : pAdrcCtx->CurrAeResult.M2S_Ratio;
    pAdrcCtx->CurrAeResult.L2M_Ratio = AecHdrPreResult.L2M_ExpRatio;
    pAdrcCtx->CurrAeResult.L2M_Ratio = pAdrcCtx->CurrAeResult.L2M_Ratio < 1 ? 1 : pAdrcCtx->CurrAeResult.L2M_Ratio;
    pAdrcCtx->CurrAeResult.DynamicRange = AecHdrPreResult.DynamicRange;
    pAdrcCtx->CurrAeResult.OEPdf = AecHdrPreResult.OverExpROIPdf[1];
    pAdrcCtx->CurrAeResult.DarkPdf = AecHdrPreResult.LowLightROIPdf[1];

    //transfer CurrAeResult data into AhdrHandle
    switch (pAdrcCtx->FrameNumber)
    {
    case 1:
        pAdrcCtx->CurrHandleData.Ratio = 1;
        pAdrcCtx->CurrAeResult.ISO = AecHdrPreResult.LinearExp.exp_real_params.analog_gain * 50.0;
        pAdrcCtx->CurrAeResult.GlobalEnvLv = AecHdrPreResult.GlobalEnvLv[0];
        pAdrcCtx->CurrAeResult.OEPdf = AecHdrPreResult.OverExpROIPdf[0];
        pAdrcCtx->CurrAeResult.DarkPdf = AecHdrPreResult.LowLightROIPdf[0];
        break;
    case 2:
        pAdrcCtx->CurrHandleData.Ratio = pAdrcCtx->CurrAeResult.M2S_Ratio;
        pAdrcCtx->CurrAeResult.ISO = AecHdrPreResult.HdrExp[1].exp_real_params.analog_gain * 50.0;
        pAdrcCtx->CurrAeResult.GlobalEnvLv = AecHdrPreResult.GlobalEnvLv[1];
        pAdrcCtx->CurrAeResult.OEPdf = AecHdrPreResult.OverExpROIPdf[1];
        pAdrcCtx->CurrAeResult.DarkPdf = AecHdrPreResult.LowLightROIPdf[1];
        break;
    case 3:
        pAdrcCtx->CurrHandleData.Ratio = pAdrcCtx->CurrAeResult.L2M_Ratio * pAdrcCtx->CurrAeResult.M2S_Ratio;
        pAdrcCtx->CurrAeResult.ISO = AecHdrPreResult.HdrExp[2].exp_real_params.analog_gain * 50.0;
        pAdrcCtx->CurrAeResult.GlobalEnvLv = AecHdrPreResult.GlobalEnvLv[2];
        pAdrcCtx->CurrAeResult.OEPdf = AecHdrPreResult.OverExpROIPdf[1];
        pAdrcCtx->CurrAeResult.DarkPdf = AecHdrPreResult.LowLightROIPdf[1];
        break;
    default:
        LOGE_AHDR("%s:  Wrong frame number in HDR mode!!!\n", __FUNCTION__);
        break;
    }

    //Normalize the current envLv for AEC
    float maxEnvLuma = 65 / 10;
    float minEnvLuma = 0;
    pAdrcCtx->CurrHandleData.EnvLv = (pAdrcCtx->CurrAeResult.GlobalEnvLv  - minEnvLuma) / (maxEnvLuma - minEnvLuma);
    pAdrcCtx->CurrHandleData.EnvLv = LIMIT_VALUE(pAdrcCtx->CurrHandleData.EnvLv, 1, 0);

    LOGD_AHDR("%s:  Current Ratio:%f\n", __FUNCTION__, pAdrcCtx->CurrHandleData.Ratio);

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

void AdrcGetCurrData
(
    AdrcHandle_t           pAdrcCtx
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_offset_pow2 = (float)(pAdrcCtx->AdrcConfig.sw_drc_offset_pow2);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_position = (float)(pAdrcCtx->AdrcConfig.sw_drc_position);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_hpdetail_ratio = (float)(pAdrcCtx->AdrcConfig.sw_drc_hpdetail_ratio);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_lpdetail_ratio = (float)(pAdrcCtx->AdrcConfig.sw_drc_lpdetail_ratio);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weicur_pix = (float)(pAdrcCtx->AdrcConfig.sw_drc_weicur_pix);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weipre_frame = (float)(pAdrcCtx->AdrcConfig.sw_drc_weipre_frame);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_force_sgm_inv0 = (float)(pAdrcCtx->AdrcConfig.sw_drc_force_sgm_inv0);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_motion_scl = (float)(pAdrcCtx->AdrcConfig.sw_drc_motion_scl);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_edge_scl = (float)(pAdrcCtx->AdrcConfig.sw_drc_edge_scl);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_space_sgm_inv1 = (float)(pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv1);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_space_sgm_inv0 = (float)(pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv0);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_range_sgm_inv1 = (float)(pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv1);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_range_sgm_inv0 = (float)(pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv0);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weig_maxl = (float)(pAdrcCtx->AdrcConfig.sw_drc_weig_maxl);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_adrc_gain = (float)(pAdrcCtx->AdrcConfig.sw_drc_gain);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weig_bilat = (float)(pAdrcCtx->AdrcConfig.sw_drc_weig_bilat);
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_min_ogain = (float)(pAdrcCtx->AdrcConfig.sw_drc_min_ogain);
    for(int i = 0; i < ISP21_DRC_Y_NUM; ++i) {
        pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_scale_y[i] = (float)(pAdrcCtx->AdrcConfig.sw_drc_scale_y[i]) ;
        pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_gain_y[i] = 0.0 ;
        pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_compres_y[i] = 0.0 ;
    }

    float iir_frame = (float)(MIN(pAdrcCtx->frameCnt + 1, pAdrcCtx->AdrcConfig.sw_drc_iir_frame));
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_iir_weight = 64.0 * (iir_frame - 1) / iir_frame;

    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_compres_scl =  20000.0;

    int ISP_RAW_BIT = 12;
    int MFHDR_LOG_Q_BITS = 11;
    int dstbits = ISP_RAW_BIT << MFHDR_LOG_Q_BITS;
    int offsetbits_int = pAdrcCtx->AdrcConfig.sw_drc_offset_pow2;
    int offsetbits = offsetbits_int << MFHDR_LOG_Q_BITS;
    int validbits = dstbits - offsetbits;
    int delta_scalein = (256 << MFHDR_LOG_Q_BITS) / validbits;
    pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_delta_scalein = (float)delta_scalein;


    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

void AdrcGetSensorInfo
(
    AdrcHandle_t     pAdrcCtx,
    AecProcResult_t  AecHdrProcResult
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    pAdrcCtx->SensorInfo.LongFrmMode = AecHdrProcResult.LongFrmMode;

    for(int i = 0; i < 3; i++)
    {
        pAdrcCtx->SensorInfo.HdrMinGain[i] = AecHdrProcResult.HdrExpRange.stGainRange[i].Min;
        pAdrcCtx->SensorInfo.HdrMaxGain[i] = AecHdrProcResult.HdrExpRange.stGainRange[i].Max;
        pAdrcCtx->SensorInfo.HdrMinIntegrationTime[i] = AecHdrProcResult.HdrExpRange.stExpTimeRange[i].Min;
        pAdrcCtx->SensorInfo.HdrMaxIntegrationTime[i] = AecHdrProcResult.HdrExpRange.stExpTimeRange[i].Max;
    }

    if(pAdrcCtx->FrameNumber == 2)
    {
        pAdrcCtx->SensorInfo.MaxExpoL = pAdrcCtx->SensorInfo.HdrMaxGain[1] * pAdrcCtx->SensorInfo.HdrMaxIntegrationTime[1];
        pAdrcCtx->SensorInfo.MinExpoL = pAdrcCtx->SensorInfo.HdrMinGain[1] * pAdrcCtx->SensorInfo.HdrMinIntegrationTime[1];
        pAdrcCtx->SensorInfo.MaxExpoM = 0;
        pAdrcCtx->SensorInfo.MinExpoM = 0;

    }
    else if(pAdrcCtx->FrameNumber == 3)
    {
        pAdrcCtx->SensorInfo.MaxExpoL = pAdrcCtx->SensorInfo.HdrMaxGain[2] * pAdrcCtx->SensorInfo.HdrMaxIntegrationTime[2];
        pAdrcCtx->SensorInfo.MinExpoL = pAdrcCtx->SensorInfo.HdrMinGain[2] * pAdrcCtx->SensorInfo.HdrMinIntegrationTime[2];
        pAdrcCtx->SensorInfo.MaxExpoM = pAdrcCtx->SensorInfo.HdrMaxGain[1] * pAdrcCtx->SensorInfo.HdrMaxIntegrationTime[1];
        pAdrcCtx->SensorInfo.MinExpoM = pAdrcCtx->SensorInfo.HdrMinGain[1] * pAdrcCtx->SensorInfo.HdrMinIntegrationTime[1];
    }


    pAdrcCtx->SensorInfo.MaxExpoS = pAdrcCtx->SensorInfo.HdrMaxGain[0] * pAdrcCtx->SensorInfo.HdrMaxIntegrationTime[0];
    pAdrcCtx->SensorInfo.MinExpoS = pAdrcCtx->SensorInfo.HdrMinGain[0] * pAdrcCtx->SensorInfo.HdrMinIntegrationTime[0];


    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcApiSetLevel()
 *
 *****************************************************************************/
void AdrcApiSetLevel
(
    AdrcHandle_t     pAdrcCtx
)
{
    LOG1_AHDR("%s:enter!\n", __FUNCTION__);



    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcIQUpdate()
 *
 *****************************************************************************/
void AdrcIQUpdate
(
    AdrcHandle_t     pAdrcCtx,
    AecPreResult_t  AecHdrPreResult,
    af_preprocess_result_t AfPreResult
)
{
    LOG1_AHDR("%s:enter!\n", __FUNCTION__);

    //get cuurrent drc data
    AdrcGetCurrData(pAdrcCtx);

    //get Current tmo TmoDamp
    pAdrcCtx->CurrHandleData.Damp = 0.3;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcTranferData2Api()
 *
 *****************************************************************************/
void AdrcTranferData2Api
(
    AdrcHandle_t     pAdrcCtx
)
{
    LOG1_AHDR("%s:enter!\n", __FUNCTION__);


    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcApiAutoUpdate()
 *
 *****************************************************************************/
void AdrcApiAutoUpdate
(
    AdrcHandle_t     pAdrcCtx
)
{
    LOG1_AHDR("%s:enter!\n", __FUNCTION__);



    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcApiManualUpdate()
 *
 *****************************************************************************/
void AdrcApiManualUpdate
(
    AdrcHandle_t     pAdrcCtx
)
{
    LOG1_AHDR("%s:enter!\n", __FUNCTION__);



    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcSelectMode()
 *transfer html parameter into handle
 ***************************************************************************/
void AdrcSelectMode
(
    AdrcHandle_t           pAdrcCtx,
    CalibDb_Adrc_Para_t*         pCalibDb,
    int mode
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    // initial checks
    DCT_ASSERT(pAdrcCtx != NULL);
    DCT_ASSERT(pCalibDb != NULL);

    pAdrcCtx->AdrcConfig.sw_drc_offset_pow2 = pCalibDb->calib[mode].sw_drc_offset_pow2;
    pAdrcCtx->AdrcConfig.sw_drc_position = pCalibDb->calib[mode].sw_drc_position;
    pAdrcCtx->AdrcConfig.sw_drc_hpdetail_ratio = pCalibDb->calib[mode].sw_drc_hpdetail_ratio;
    pAdrcCtx->AdrcConfig.sw_drc_lpdetail_ratio = pCalibDb->calib[mode].sw_drc_lpdetail_ratio;
    pAdrcCtx->AdrcConfig.sw_drc_weicur_pix = pCalibDb->calib[mode].sw_drc_weicur_pix;
    pAdrcCtx->AdrcConfig.sw_drc_weipre_frame = pCalibDb->calib[mode].sw_drc_weipre_frame;
    pAdrcCtx->AdrcConfig.sw_drc_force_sgm_inv0 = pCalibDb->calib[mode].sw_drc_force_sgm_inv0;
    pAdrcCtx->AdrcConfig.sw_drc_motion_scl = pCalibDb->calib[mode].sw_drc_motion_scl;
    pAdrcCtx->AdrcConfig.sw_drc_edge_scl = pCalibDb->calib[mode].sw_drc_edge_scl;
    pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv1 = pCalibDb->calib[mode].sw_drc_space_sgm_inv1;
    pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv0 = pCalibDb->calib[mode].sw_drc_space_sgm_inv0;
    pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv1 = pCalibDb->calib[mode].sw_drc_range_sgm_inv1;
    pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv0 = pCalibDb->calib[mode].sw_drc_range_sgm_inv0;
    pAdrcCtx->AdrcConfig.sw_drc_weig_maxl = pCalibDb->calib[mode].sw_drc_weig_maxl;
    pAdrcCtx->AdrcConfig.sw_drc_weig_bilat = pCalibDb->calib[mode].sw_drc_weig_bilat;
    pAdrcCtx->AdrcConfig.sw_drc_iir_frame = pCalibDb->calib[mode].sw_drc_iir_frame;
    pAdrcCtx->AdrcConfig.sw_drc_gain = pCalibDb->calib[mode].sw_drc_gain;
    pAdrcCtx->AdrcConfig.sw_drc_min_ogain = pCalibDb->calib[mode].sw_drc_min_ogain;
    for(int i = 0; i < ISP21_DRC_Y_NUM; ++i)
        pAdrcCtx->AdrcConfig.sw_drc_scale_y[i] = pCalibDb->calib[mode].sw_drc_scale_y[i] ;
#if 0
    LOGE_AHDR("%s:  sw_drc_offset_pow2:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_offset_pow2);
    LOGE_AHDR("%s:  sw_drc_position:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_position);
    LOGE_AHDR("%s:  sw_drc_hpdetail_ratio:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_hpdetail_ratio);
    LOGE_AHDR("%s:  sw_drc_lpdetail_ratio:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_lpdetail_ratio);
    LOGE_AHDR("%s:  sw_drc_weicur_pix:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_weicur_pix);
    LOGE_AHDR("%s:  sw_drc_weipre_frame:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_weipre_frame);
    LOGE_AHDR("%s:  sw_drc_force_sgm_inv0:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_force_sgm_inv0);
    LOGE_AHDR("%s:  sw_drc_motion_scl:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_motion_scl);
    LOGE_AHDR("%s:  sw_drc_edge_scl:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_edge_scl);
    LOGE_AHDR("%s:  sw_drc_space_sgm_inv1:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv1);
    LOGE_AHDR("%s:  sw_drc_space_sgm_inv0:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_space_sgm_inv0);
    LOGE_AHDR("%s:  sw_drc_range_sgm_inv1:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv1);
    LOGE_AHDR("%s:  sw_drc_range_sgm_inv0:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_range_sgm_inv0);
    LOGE_AHDR("%s:  sw_drc_weig_maxl:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_weig_maxl);
    LOGE_AHDR("%s:  sw_drc_gain:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_gain);
    LOGE_AHDR("%s:  sw_drc_iir_frame:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_iir_frame);
    LOGE_AHDR("%s:  sw_drc_min_ogain:%d\n", __FUNCTION__, pAdrcCtx->AdrcConfig.sw_drc_min_ogain);

#endif
    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}
/******************************************************************************
 * AdrcUpdateConfig()
 *get handle para by config and current variate
 *****************************************************************************/
void AdrcUpdateConfig
(
    AdrcHandle_t     pAdrcCtx,
    AecPreResult_t  AecHdrPreResult,
    af_preprocess_result_t AfPreResult
)
{
    LOG1_AHDR("%s:enter!\n", __FUNCTION__);

    LOGD_AHDR("%s:  Ahdr Current frame cnt:%d:\n",  __FUNCTION__, pAdrcCtx->frameCnt);

    //get current ae data from AecPreRes
    AdrcGetAeResult(pAdrcCtx, AecHdrPreResult);

    //transfer ae data to CurrHandle
    pAdrcCtx->CurrHandleData.EnvLv = LIMIT_VALUE(pAdrcCtx->CurrHandleData.EnvLv, ENVLVMAX, ENVLVMIN);

    pAdrcCtx->CurrHandleData.MoveCoef = 1;
    pAdrcCtx->CurrHandleData.MoveCoef = LIMIT_VALUE(pAdrcCtx->CurrHandleData.MoveCoef, MOVECOEFMAX, MOVECOEFMIN);

    pAdrcCtx->CurrHandleData.ISO = pAdrcCtx->CurrAeResult.ISO;
    pAdrcCtx->CurrHandleData.ISO = LIMIT_VALUE(pAdrcCtx->CurrHandleData.ISO, ISOMAX, ISOMIN);

    pAdrcCtx->CurrHandleData.OEPdf = pAdrcCtx->CurrAeResult.OEPdf;
    pAdrcCtx->CurrHandleData.OEPdf = LIMIT_VALUE(pAdrcCtx->CurrHandleData.OEPdf, OEPDFMAX, OEPDFMIN);

    pAdrcCtx->CurrHandleData.TotalFocusLuma = 1;
    pAdrcCtx->CurrHandleData.TotalFocusLuma = LIMIT_VALUE(pAdrcCtx->CurrHandleData.TotalFocusLuma, FOCUSLUMAMAX, FOCUSLUMAMIN);

    pAdrcCtx->CurrHandleData.DarkPdf = pAdrcCtx->CurrAeResult.DarkPdf;
    pAdrcCtx->CurrHandleData.DarkPdf = LIMIT_VALUE(pAdrcCtx->CurrHandleData.DarkPdf, 0.5, 0);

    pAdrcCtx->CurrHandleData.DynamicRange = pAdrcCtx->CurrAeResult.DynamicRange;
    pAdrcCtx->CurrHandleData.DynamicRange = LIMIT_VALUE(pAdrcCtx->CurrHandleData.DynamicRange, DYNAMICRANGEMAX, DYNAMICRANGEMIN);

    pAdrcCtx->drcAttr.opMode = DRC_OPMODE_API_OFF;
    if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_API_OFF)
    {
        LOGD_AHDR("%s:  Ahdr api OFF!! Current Handle data:\n", __FUNCTION__);

        AdrcIQUpdate(pAdrcCtx, AecHdrPreResult, AfPreResult);

    }
    else if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_AUTO)
    {
        LOGD_AHDR("%s:  Ahdr api Auto!! Current Handle data:\n", __FUNCTION__);
        //AdrcApiAutoUpdate(AdrcHandle_t pAdrcCtx)(pAdrcCtx);
    }
    else if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_MANU)
    {
        LOGD_AHDR("%s:  Ahdr api Manual!! Current Handle data:\n", __FUNCTION__);
        //AdrcApiManualUpdate(pAdrcCtx);
    }
    else if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_SET_LEVEL)
    {
        LOGD_AHDR("%s:  Ahdr api set level!! Current Handle data:\n", __FUNCTION__);

        AdrcIQUpdate(pAdrcCtx, AecHdrPreResult, AfPreResult);
        //AdrcApiSetLevel(pAdrcCtx);

    }
    else if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_DARKAREA)
    {
        LOGD_AHDR("%s:  Ahdr api DarkArea!! Current Handle data:\n", __FUNCTION__);
        AdrcIQUpdate(pAdrcCtx, AecHdrPreResult, AfPreResult);


    }
    else if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_TOOL)
    {
        LOGD_AHDR("%s:  Ahdr api Tool!! Current Handle data:\n", __FUNCTION__);
        AdrcIQUpdate(pAdrcCtx, AecHdrPreResult, AfPreResult);
    }
    else
        LOGE_AHDR("%s:  Ahdr wrong mode!!!\n", __FUNCTION__);

    //transfer data to api
    AdrcTranferData2Api(pAdrcCtx);

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}
/******************************************************************************
 * DrcSetGlobalTMO()
 *****************************************************************************/
bool DrcSetGlobalTMO
(
    AdrcHandle_t pAdrcCtx
) {

    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    bool returnValue = false;



    return returnValue;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcInit()
 *****************************************************************************/
XCamReturn AdrcInit
(
    AdrcInstanceConfig_t* pInstConfig
) {

    AdrcContext_s *pAdrcCtx;

    LOG1_AHDR("%s:enter!\n", __FUNCTION__);

    XCamReturn result = XCAM_RETURN_NO_ERROR;

    // initial checks
    if (pInstConfig == NULL)
        return (XCAM_RETURN_ERROR_PARAM);

    // allocate Adrc control context
    pAdrcCtx = (AdrcContext_s*)malloc(sizeof(AdrcContext_t));
    if (NULL == pAdrcCtx) {
        LOGE_AHDR( "%s: Can't allocate Adrc context\n",  __FUNCTION__);
        return (XCAM_RETURN_ERROR_OUTOFRANGE);
    }

    // pre-initialize context
    memset(pAdrcCtx, 0x00, sizeof(*pAdrcCtx));

    pAdrcCtx->state = ADRC_STATE_INITIALIZED;

    // memory check
    if (NULL == pAdrcCtx) {
        return (XCAM_RETURN_ERROR_FILE);
    }
    // state check
    if ((ADRC_STATE_INITIALIZED != pAdrcCtx->state)
            && (ADRC_STATE_STOPPED != pAdrcCtx->state)) {
        return (XCAM_RETURN_ERROR_MEM);
    }

    pInstConfig->hAdrc = (AdrcHandle_t)pAdrcCtx;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);

    return (XCAM_RETURN_NO_ERROR);
}
/******************************************************************************
 * AhdrRelease()
 *****************************************************************************/
XCamReturn AdrcRelease
(
    AdrcHandle_t pAdrcCtx
) {

    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    // initial checks
    if (NULL == pAdrcCtx) {
        return (XCAM_RETURN_ERROR_PARAM);
    }

    result = AdrcStop(pAdrcCtx);
    if (result != XCAM_RETURN_NO_ERROR) {
        LOGE_AHDR( "%s: AHDRStop() failed!\n", __FUNCTION__);
        return (result);
    }

    // check state
    if ((ADRC_STATE_RUNNING == pAdrcCtx->state)
            || (ADRC_STATE_LOCKED == pAdrcCtx->state)) {
        return (XCAM_RETURN_ERROR_MEM);
    }

    memset(pAdrcCtx, 0, sizeof(AdrcContext_t));
    free(pAdrcCtx);
    pAdrcCtx = NULL;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);

    return (XCAM_RETURN_NO_ERROR);
}
/******************************************************************************
 * DrcGetCurrIOData()
 *****************************************************************************/
void DrcGetCurrIOData
(
    AdrcHandle_t pAdrcCtx
)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);

    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_offset_pow2 = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_offset_pow2);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_compres_scl =  (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_compres_scl);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_position = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_position);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_delta_scalein = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_delta_scalein);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_hpdetail_ratio = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_hpdetail_ratio);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_lpdetail_ratio = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_lpdetail_ratio);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weicur_pix = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weicur_pix);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weipre_frame = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weipre_frame);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_force_sgm_inv0 = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_force_sgm_inv0);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_motion_scl = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_motion_scl);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_edge_scl = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_edge_scl);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_space_sgm_inv1 = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_space_sgm_inv1);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_space_sgm_inv0 = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_space_sgm_inv0);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_range_sgm_inv1 = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_range_sgm_inv1);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_range_sgm_inv0 = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_range_sgm_inv0);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weig_maxl = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weig_maxl);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weig_bilat = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_weig_bilat);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_adrc_gain = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_adrc_gain * 1024 + 0.5);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_iir_weight = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_iir_weight);
    pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_min_ogain = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_min_ogain);
    for(int i = 0; i < ISP21_DRC_Y_NUM; ++i)
    {
        pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_gain_y[i] = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_gain_y[i]) ;
        pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_compres_y[i] = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_compres_y[i]) ;
        pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_scale_y[i] = (int)(pAdrcCtx->CurrHandleData.DrcHandleData.sw_drc_scale_y[i]) ;
    }

    //get sw_drc_gain_y
    CalibrateDrcGainY(&pAdrcCtx->AdrcProcRes.DrcProcRes) ;

    LOGD_AHDR("%s:  sw_drc_offset_pow2:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_offset_pow2);
    LOGD_AHDR("%s:  sw_drc_compres_scl:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_compres_scl);
    LOGD_AHDR("%s:  sw_drc_position:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_position);
    LOGD_AHDR("%s:  sw_drc_delta_scalein:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_delta_scalein);
    LOGD_AHDR("%s:  sw_drc_hpdetail_ratio:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_hpdetail_ratio);
    LOGD_AHDR("%s:  sw_drc_lpdetail_ratio:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_lpdetail_ratio);
    LOGD_AHDR("%s:  sw_drc_weicur_pix:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weicur_pix);
    LOGD_AHDR("%s:  sw_drc_weipre_frame:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weipre_frame);
    LOGD_AHDR("%s:  sw_drc_force_sgm_inv0:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_force_sgm_inv0);
    LOGD_AHDR("%s:  sw_drc_motion_scl:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_motion_scl);
    LOGD_AHDR("%s:  sw_drc_edge_scl:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_edge_scl);
    LOGD_AHDR("%s:  sw_drc_space_sgm_inv1:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_space_sgm_inv1);
    LOGD_AHDR("%s:  sw_drc_space_sgm_inv0:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_space_sgm_inv0);
    LOGD_AHDR("%s:  sw_drc_range_sgm_inv1:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_range_sgm_inv1);
    LOGD_AHDR("%s:  sw_drc_range_sgm_inv0:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_range_sgm_inv0);
    LOGD_AHDR("%s:  sw_drc_weig_maxl:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weig_maxl);
    LOGD_AHDR("%s:  sw_drc_weig_bilat:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_weig_bilat);
    LOGD_AHDR("%s:  sw_drc_adrc_gain:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_adrc_gain);
    LOGD_AHDR("%s:  sw_drc_iir_weight:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_iir_weight);
    LOGD_AHDR("%s:  sw_drc_min_ogain:%d\n", __FUNCTION__, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_min_ogain);
    for(int i = 0; i < 17; i++)
        LOGD_AHDR("%s:  sw_drc_gain_y[%d]:%d\n", __FUNCTION__, i, pAdrcCtx->AdrcProcRes.DrcProcRes.sw_drc_gain_y[i]);

    LOG1_AHDR("%s:Eixt!\n", __FUNCTION__);
}

/******************************************************************************
 * DrcProcessing()
 *****************************************************************************/
void DrcProcessing
(
    AdrcHandle_t pAdrcCtx
)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);

    //damp


    //get current IO data
    DrcGetCurrIOData(pAdrcCtx);

    LOG1_AHDR("%s:Eixt!\n", __FUNCTION__);
}

/******************************************************************************
 * AdrcProcessing()
 *****************************************************************************/
void AdrcProcessing
(
    AdrcHandle_t pAdrcCtx
) {

    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);

    DrcProcessing(pAdrcCtx);

    // Set Global TMO
    //pAdrcCtx->AdrcProcRes.isHdrGlobalTmo = DrcSetGlobalTMO(AdrcHandle_t pAdrcCtx)(pAdrcCtx);

    // store current handle data to pre data for next loop
    pAdrcCtx->AdrcPrevData.Ratio = pAdrcCtx->CurrHandleData.Ratio;
    pAdrcCtx->AdrcPrevData.Envlv = pAdrcCtx->CurrHandleData.EnvLv;
    pAdrcCtx->AdrcPrevData.MoveCoef = pAdrcCtx->CurrHandleData.MoveCoef;
    pAdrcCtx->AdrcPrevData.OEPdf = pAdrcCtx->CurrHandleData.OEPdf;
    pAdrcCtx->AdrcPrevData.TotalFocusLuma = pAdrcCtx->CurrHandleData.TotalFocusLuma;
    pAdrcCtx->AdrcPrevData.DarkPdf = pAdrcCtx->CurrHandleData.DarkPdf;
    pAdrcCtx->AdrcPrevData.ISO = pAdrcCtx->CurrHandleData.ISO;
    pAdrcCtx->AdrcPrevData.DynamicRange = pAdrcCtx->CurrHandleData.DynamicRange;
    memcpy(&pAdrcCtx->AdrcPrevData.DrcHandleData, &pAdrcCtx->CurrHandleData.DrcHandleData, sizeof(DrcHandleData_t));
    ++pAdrcCtx->frameCnt;

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}
