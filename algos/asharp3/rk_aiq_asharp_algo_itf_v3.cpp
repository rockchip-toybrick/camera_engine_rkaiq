/*
 * rk_aiq_algo_anr_itf.c
 *
 *  Copyright (c) 2019 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "rk_aiq_algo_types_int.h"
#include "asharp3/rk_aiq_asharp_algo_itf_v3.h"
#include "asharp3/rk_aiq_asharp_algo_v3.h"

RKAIQ_BEGIN_DECLARE

typedef struct _RkAiqAlgoContext {
    void* place_holder[0];
} RkAiqAlgoContext;


static XCamReturn
create_context(RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg)
{

    XCamReturn result = XCAM_RETURN_NO_ERROR;
    AlgoCtxInstanceCfgInt *cfgInt = (AlgoCtxInstanceCfgInt*)cfg;
    LOGI_ASHARP("%s:oyyf (enter)\n", __FUNCTION__ );

#if 1
    Asharp_Context_V3_t* pAsharpCtx = NULL;
    Asharp3_result_t ret = Asharp_Init_V3(&pAsharpCtx, cfgInt->calib);
    if(ret != ASHARP3_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ASHARP("%s: Initializaion ANR failed (%d)\n", __FUNCTION__, ret);
    } else {
        *context = (RkAiqAlgoContext *)(pAsharpCtx);
    }
#endif

    LOGI_ASHARP("%s:oyyf (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
destroy_context(RkAiqAlgoContext *context)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGI_ASHARP("%s: oyyf (enter)\n", __FUNCTION__ );

#if 1
    Asharp_Context_V3_t* pAsharpCtx = (Asharp_Context_V3_t*)context;
    Asharp3_result_t ret = Asharp_Release_V3(pAsharpCtx);
    if(ret != ASHARP3_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ASHARP("%s: release ANR failed (%d)\n", __FUNCTION__, ret);
    }
#endif

    LOGI_ASHARP("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
prepare(RkAiqAlgoCom* params)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGI_ASHARP("%s: oyyf (enter)\n", __FUNCTION__ );

    Asharp_Context_V3_t* pAsharpCtx = (Asharp_Context_V3_t *)params->ctx;
    RkAiqAlgoConfigAsharpV3Int* pCfgParam = (RkAiqAlgoConfigAsharpV3Int*)params;

    Asharp3_result_t ret = Asharp_Prepare_V3(pAsharpCtx, &pCfgParam->stAsharpConfig);
    if(ret != ASHARP3_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ASHARP("%s: config ANR failed (%d)\n", __FUNCTION__, ret);
    }

    LOGI_ASHARP("%s: oyyf (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
pre_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGD_ASHARP("%s: oyyf (enter)\n", __FUNCTION__ );
    Asharp_Context_V3_t* pAsharpCtx = (Asharp_Context_V3_t *)inparams->ctx;
	
    RkAiqAlgoPreAsharpV3Int* pAsharpPreParams = (RkAiqAlgoPreAsharpV3Int*)inparams;

    if (pAsharpPreParams->rk_com.u.proc.gray_mode) {
        pAsharpCtx->isGrayMode = true;
    }else {
        pAsharpCtx->isGrayMode = false;
    }

    Asharp3_result_t ret = Asharp_PreProcess_V3(pAsharpCtx);
    if(ret != ASHARP3_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ASHARP("%s: ANRPreProcess failed (%d)\n", __FUNCTION__, ret);
    }

    LOGD_ASHARP("%s: oyyf (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
processing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGD_ASHARP("%s:oyyf (enter)\n", __FUNCTION__ );

#if 1
    RkAiqAlgoProcAsharpV3Int* pAsharpProcParams = (RkAiqAlgoProcAsharpV3Int*)inparams;
    RkAiqAlgoProcResAsharpV3Int* pAsharpProcResParams = (RkAiqAlgoProcResAsharpV3Int*)outparams;
    Asharp_Context_V3_t* pAsharpCtx = (Asharp_Context_V3_t *)inparams->ctx;
    Asharp3_ExpInfo_t stExpInfo;
    memset(&stExpInfo, 0x00, sizeof(Asharp3_ExpInfo_t));

    LOGD_ASHARP("%s:%d init:%d hdr mode:%d  \n",
             __FUNCTION__, __LINE__,
             inparams->u.proc.init,
             pAsharpProcParams->hdr_mode);

    stExpInfo.hdr_mode = 0; //pAsharpProcParams->hdr_mode;
    for(int i = 0; i < 3; i++) {
        stExpInfo.arIso[i] = 50;
        stExpInfo.arAGain[i] = 1.0;
        stExpInfo.arDGain[i] = 1.0;
        stExpInfo.arTime[i] = 0.01;
    }

    if(pAsharpProcParams->hdr_mode == RK_AIQ_WORKING_MODE_NORMAL) {
        stExpInfo.hdr_mode = 0;
    } else if(pAsharpProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_2_FRAME_HDR
              || pAsharpProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_2_LINE_HDR ) {
        stExpInfo.hdr_mode = 1;
    } else if(pAsharpProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_3_FRAME_HDR
              || pAsharpProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_3_LINE_HDR ) {
        stExpInfo.hdr_mode = 2;
    }
	stExpInfo.snr_mode = 0;

#if 1
    RkAiqAlgoPreResAeInt* pAEPreRes =
        (RkAiqAlgoPreResAeInt*)(pAsharpProcParams->rk_com.u.proc.pre_res_comb->ae_pre_res);

    if(pAEPreRes != NULL) {
	stExpInfo.snr_mode = pAEPreRes->ae_pre_res_rk.CISFeature.SNR;
        if(pAsharpProcParams->hdr_mode == RK_AIQ_WORKING_MODE_NORMAL) {
            stExpInfo.hdr_mode = 0;
            stExpInfo.arAGain[0] = pAEPreRes->ae_pre_res_rk.LinearExp.exp_real_params.analog_gain;
            stExpInfo.arDGain[0] = pAEPreRes->ae_pre_res_rk.LinearExp.exp_real_params.digital_gain;
            stExpInfo.arTime[0] = pAEPreRes->ae_pre_res_rk.LinearExp.exp_real_params.integration_time;
            stExpInfo.arIso[0] = stExpInfo.arAGain[0] * stExpInfo.arDGain[0] * 50;
        } else {
            for(int i = 0; i < 3; i++) {
                stExpInfo.arAGain[i] = pAEPreRes->ae_pre_res_rk.HdrExp[i].exp_real_params.analog_gain;
                stExpInfo.arDGain[i] = pAEPreRes->ae_pre_res_rk.HdrExp[i].exp_real_params.digital_gain;
                stExpInfo.arTime[i] = pAEPreRes->ae_pre_res_rk.HdrExp[i].exp_real_params.integration_time;
                stExpInfo.arIso[i] = stExpInfo.arAGain[i] * stExpInfo.arDGain[i] * 50;

                LOGD_ASHARP("%s:%d index:%d again:%f dgain:%f time:%f iso:%d hdr_mode:%d\n",
                         __FUNCTION__, __LINE__,
                         i,
                         stExpInfo.arAGain[i],
                         stExpInfo.arDGain[i],
                         stExpInfo.arTime[i],
                         stExpInfo.arIso[i],
                         stExpInfo.hdr_mode);
            }
        }
    } else {
        LOGE_ASHARP("%s:%d pAEPreRes is NULL, so use default instead \n", __FUNCTION__, __LINE__);
    }

#if 0
    static int anr_cnt = 0;
    anr_cnt++;

    if(anr_cnt % 50 == 0) {
        for(int i = 0; i < stExpInfo.hdr_mode + 1; i++) {
            printf("%s:%d index:%d again:%f dgain:%f time:%f iso:%d hdr_mode:%d\n",
                   __FUNCTION__, __LINE__,
                   i,
                   stExpInfo.arAGain[i],
                   stExpInfo.arDGain[i],
                   stExpInfo.arTime[i],
                   stExpInfo.arIso[i],
                   stExpInfo.hdr_mode);
        }
    }
#endif


#endif

    Asharp3_result_t ret = Asharp_Process_V3(pAsharpCtx, &stExpInfo);
    if(ret != ASHARP3_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ASHARP("%s: processing ANR failed (%d)\n", __FUNCTION__, ret);
    }

    Asharp_GetProcResult_V3(pAsharpCtx, &pAsharpProcResParams->stAsharpProcResult);
#endif

    LOGD_ASHARP("%s: oyyf(exit)\n", __FUNCTION__ );
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn
post_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOGI_ASHARP("%s: (enter)\n", __FUNCTION__ );

    //nothing todo now

    LOGI_ASHARP("%s: (exit)\n", __FUNCTION__ );
    return XCAM_RETURN_NO_ERROR;
}

RkAiqAlgoDescription g_RkIspAlgoDescAsharpV3 = {
    .common = {
        .version = RKISP_ALGO_ASHARP_VERSION_V3,
        .vendor  = RKISP_ALGO_ASHARP_VENDOR_V3,
        .description = RKISP_ALGO_ASHARP_DESCRIPTION_V3,
        .type    = RK_AIQ_ALGO_TYPE_ASHARP,
        .id      = 0,
        .create_context  = create_context,
        .destroy_context = destroy_context,
    },
    .prepare = prepare,
    .pre_process = pre_process,
    .processing = processing,
    .post_process = post_process,
};

RKAIQ_END_DECLARE
