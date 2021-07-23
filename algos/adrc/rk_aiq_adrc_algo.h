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
#ifndef __RK_AIQ_ADRC_ALGO_H__
#define __RK_AIQ_ADRC_ALGO_H__

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ae/rk_aiq_types_ae_algo_int.h"
#include "af/rk_aiq_af_hw_v200.h"
//#include "rk_aiq_uapi_adrc_int.h"
#include "rk_aiq_types_adrc_algo_prvt.h"


XCamReturn AdrcStart(AdrcHandle_t pAdrcCtx);
XCamReturn AdrcStop(AdrcHandle_t pAdrcCtx);
void AdrcApiOffConfig(AdrcHandle_t pAdrcCtx);
void AdrcConfig(AdrcHandle_t pAdrcCtx);
void AdrcGetStats(AdrcHandle_t pAdrcCtx, rkisp_adrc_stats_t* ROData) ;
void AdrcIQUpdate(AdrcHandle_t pAdrcCtx, AecPreResult_t AecHdrPreResult, af_preprocess_result_t AfPreResult);
void AdrcGetSensorInfo( AdrcHandle_t pAdrcCtx, AecProcResult_t AecHdrProcResult);
void AdrcSelectMode(AdrcHandle_t pAdrcCtx, CalibDb_Adrc_Para_t* pConfig, int mode);
void AdrcUpdateConfig(AdrcHandle_t pAdrcCtx, AecPreResult_t AecHdrPreResult, af_preprocess_result_t AfPreResult);
bool DrcSetGlobalTMO(AdrcHandle_t pAdrcCtx);
XCamReturn AdrcInit(AdrcInstanceConfig_t* pInstConfig) ;
XCamReturn AdrcRelease(AdrcHandle_t pAdrcCtx) ;
void AdrcProcessing(AdrcHandle_t pAdrcCtx);


#endif
