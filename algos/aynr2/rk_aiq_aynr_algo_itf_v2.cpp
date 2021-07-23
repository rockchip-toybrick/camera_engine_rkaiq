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
#include "aynr2/rk_aiq_aynr_algo_itf_v2.h"
#include "aynr2/rk_aiq_aynr_algo_v2.h"

RKAIQ_BEGIN_DECLARE

typedef struct _RkAiqAlgoContext {
    void* place_holder[0];
} RkAiqAlgoContext;


static XCamReturn
create_context(RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg)
{

    XCamReturn result = XCAM_RETURN_NO_ERROR;
    AlgoCtxInstanceCfgInt *cfgInt = (AlgoCtxInstanceCfgInt*)cfg;
    LOGI_ANR("%s: (enter)\n", __FUNCTION__ );

#if 1
    Aynr_Context_V2_t* pAynrCtx = NULL;
    Aynr_result_t ret = Aynr_Init_V2(&pAynrCtx, cfgInt->calib);
    if(ret != AYNR_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ANR("%s: Initializaion ANR failed (%d)\n", __FUNCTION__, ret);
    } else {
        *context = (RkAiqAlgoContext *)(pAynrCtx);
    }
#endif

    LOGI_ANR("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
destroy_context(RkAiqAlgoContext *context)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGI_ANR("%s: (enter)\n", __FUNCTION__ );

#if 1
    Aynr_Context_V2_t* pAynrCtx = (Aynr_Context_V2_t*)context;
    Aynr_result_t ret = Aynr_Release_V2(pAynrCtx);
    if(ret != AYNR_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ANR("%s: release ANR failed (%d)\n", __FUNCTION__, ret);
    }
#endif

    LOGI_ANR("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
prepare(RkAiqAlgoCom* params)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGI_ANR("%s: (enter)\n", __FUNCTION__ );

    Aynr_Context_V2_t* pAynrCtx = (Aynr_Context_V2_t *)params->ctx;
    RkAiqAlgoConfigAynrV2Int* pCfgParam = (RkAiqAlgoConfigAynrV2Int*)params;

    Aynr_result_t ret = Aynr_Prepare_V2(pAynrCtx, &pCfgParam->stAynrConfig);
    if(ret != AYNR_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ANR("%s: config ANR failed (%d)\n", __FUNCTION__, ret);
    }

    LOGI_ANR("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
pre_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGI_ANR("%s: (enter)\n", __FUNCTION__ );
    Aynr_Context_V2_t* pAynrCtx = (Aynr_Context_V2_t *)inparams->ctx;
	
    RkAiqAlgoPreAynrV2Int* pAnrPreParams = (RkAiqAlgoPreAynrV2Int*)inparams;

    if (pAnrPreParams->rk_com.u.proc.gray_mode) {
        pAynrCtx->isGrayMode = true;
    }else {
        pAynrCtx->isGrayMode = false;
    }

    Aynr_result_t ret = Aynr_PreProcess_V2(pAynrCtx);
    if(ret != AYNR_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ANR("%s: ANRPreProcess failed (%d)\n", __FUNCTION__, ret);
    }

    LOGI_ANR("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
processing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    LOGI_ANR("%s: (enter)\n", __FUNCTION__ );

#if 1
    RkAiqAlgoProcAynrV2Int* pAynrProcParams = (RkAiqAlgoProcAynrV2Int*)inparams;
    RkAiqAlgoProcResAynrV2Int* pAynrProcResParams = (RkAiqAlgoProcResAynrV2Int*)outparams;
    Aynr_Context_V2_t* pAynrCtx = (Aynr_Context_V2_t *)inparams->ctx;
    Aynr_ExpInfo_t stExpInfo;
    memset(&stExpInfo, 0x00, sizeof(Aynr_ExpInfo_t));

    LOGD_ANR("%s:%d init:%d hdr mode:%d  \n",
             __FUNCTION__, __LINE__,
             inparams->u.proc.init,
             pAynrProcParams->hdr_mode);

    stExpInfo.hdr_mode = 0; //pAnrProcParams->hdr_mode;
    for(int i = 0; i < 3; i++) {
        stExpInfo.arIso[i] = 50;
        stExpInfo.arAGain[i] = 1.0;
        stExpInfo.arDGain[i] = 1.0;
        stExpInfo.arTime[i] = 0.01;
    }

    if(pAynrProcParams->hdr_mode == RK_AIQ_WORKING_MODE_NORMAL) {
        stExpInfo.hdr_mode = 0;
    } else if(pAynrProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_2_FRAME_HDR
              || pAynrProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_2_LINE_HDR ) {
        stExpInfo.hdr_mode = 1;
    } else if(pAynrProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_3_FRAME_HDR
              || pAynrProcParams->hdr_mode == RK_AIQ_ISP_HDR_MODE_3_LINE_HDR ) {
        stExpInfo.hdr_mode = 2;
    }
	stExpInfo.snr_mode = 0;

#if 1
    RkAiqAlgoPreResAeInt* pAEPreRes =
        (RkAiqAlgoPreResAeInt*)(pAynrProcParams->rk_com.u.proc.pre_res_comb->ae_pre_res);

    if(pAEPreRes != NULL) {
	stExpInfo.snr_mode = pAEPreRes->ae_pre_res_rk.CISFeature.SNR;
        if(pAynrProcParams->hdr_mode == RK_AIQ_WORKING_MODE_NORMAL) {
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

                LOGD_ANR("%s:%d index:%d again:%f dgain:%f time:%f iso:%d hdr_mode:%d\n",
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
        LOGE_ANR("%s:%d pAEPreRes is NULL, so use default instead \n", __FUNCTION__, __LINE__);
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

    Aynr_result_t ret = Aynr_Process_V2(pAynrCtx, &stExpInfo);
    if(ret != AYNR_RET_SUCCESS) {
        result = XCAM_RETURN_ERROR_FAILED;
        LOGE_ANR("%s: processing ANR failed (%d)\n", __FUNCTION__, ret);
    }

    Aynr_GetProcResult_V2(pAynrCtx, &pAynrProcResParams->stAynrProcResult);
#endif

    LOGI_ANR("%s: (exit)\n", __FUNCTION__ );
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn
post_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOGI_ANR("%s: (enter)\n", __FUNCTION__ );

    //nothing todo now

    LOGI_ANR("%s: (exit)\n", __FUNCTION__ );
    return XCAM_RETURN_NO_ERROR;
}

RkAiqAlgoDescription g_RkIspAlgoDescAynrV2 = {
    .common = {
        .version = RKISP_ALGO_AYNR_VERSION_V2,
        .vendor  = RKISP_ALGO_AYNR_VENDOR_V2,
        .description = RKISP_ALGO_AYNR_DESCRIPTION_V2,
        .type    = RK_AIQ_ALGO_TYPE_AYNR,
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
