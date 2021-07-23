/*
 * rk_aiq_algo_adrc_itf.c
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
#include "adrc/rk_aiq_algo_adrc_itf.h"
#include "xcam_log.h"
#include "adrc/rk_aiq_adrc_algo.h"
#include "adrc/rk_aiq_types_adrc_algo_prvt.h"

RKAIQ_BEGIN_DECLARE


static XCamReturn
create_context(RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg)
{   
LOG1_AHDR("lrk %s:Enter!\n", __FUNCTION__);
XCamReturn result = XCAM_RETURN_NO_ERROR;
    RkAiqAlgoContext *ctx = new RkAiqAlgoContext();
    if (ctx == NULL) {
        LOGE_AHDR( "%s: create adrc context fail!\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_MEM;
    }
    memset(&ctx->AdrcInstConfig, 0x00, sizeof(AdrcInstanceConfig_t));
    result = AdrcInit(&ctx->AdrcInstConfig);

    if (result != XCAM_RETURN_NO_ERROR) {
        LOGE_AHDR("%s Adrc Init failed: %d", __FUNCTION__, result);
        return(XCAM_RETURN_ERROR_FAILED);
    }

    *context = ctx;

    LOG1_AHDR(" lrk %s:Exit!\n", __FUNCTION__);
    return result;
}

static XCamReturn
destroy_context(RkAiqAlgoContext *context)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    if(context != NULL) {

        AdrcHandle_t pAdrcCtx = (AdrcHandle_t)context->AdrcInstConfig.hAdrc;
        result = AdrcRelease(pAdrcCtx);
        if (result != XCAM_RETURN_NO_ERROR) {
            LOGE_AHDR("%s Adrc Release failed: %d", __FUNCTION__, result);
            return(XCAM_RETURN_ERROR_FAILED);
        }
        delete context;
        context = NULL;
    }

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return result;
}

static XCamReturn
prepare(RkAiqAlgoCom* params)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    AdrcHandle_t pAdrcCtx = params->ctx->AdrcInstConfig.hAdrc;
    RkAiqAlgoConfigAdrcInt* AdrcCfgParam = (RkAiqAlgoConfigAdrcInt*)params; //come from params in html
    const CamCalibDbContext_t* pCalibDb = AdrcCfgParam->rk_com.u.prepare.calib;
    pAdrcCtx->width = AdrcCfgParam->rawWidth;
    pAdrcCtx->height = AdrcCfgParam->rawHeight;

    if (AdrcCfgParam->working_mode < RK_AIQ_WORKING_MODE_ISP_HDR2)
        pAdrcCtx->FrameNumber = 1;
    else if (AdrcCfgParam->working_mode < RK_AIQ_WORKING_MODE_ISP_HDR3 &&
             AdrcCfgParam->working_mode >= RK_AIQ_WORKING_MODE_ISP_HDR2)
        pAdrcCtx->FrameNumber = 2;
    else
        pAdrcCtx->FrameNumber = 3;

    AdrcConfig(pAdrcCtx); //set default para
    memcpy(&pAdrcCtx->pCalibDB, &pCalibDb->adrc, sizeof(CalibDb_Adrc_Para_t));//load iq paras
    memcpy(&pAdrcCtx->drcAttr.stTool, &pCalibDb->adrc, sizeof(CalibDb_Adrc_Para_t));//load iq paras to stTool

    if (result != XCAM_RETURN_NO_ERROR) {
        LOGE_AHDR("%s Adrc UpdateConfig failed: %d", __FUNCTION__, result);
        return(XCAM_RETURN_ERROR_FAILED);
    }

    if(/* !params->u.prepare.reconfig*/true) {
        AdrcStop(pAdrcCtx); // stop firstly for re-preapre
        result = AdrcStart(pAdrcCtx);
        if (result != XCAM_RETURN_NO_ERROR) {
            LOGE_AHDR("%s Adrc Start failed: %d", __FUNCTION__, result);
            return(XCAM_RETURN_ERROR_FAILED);
        }
    }
    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return result;
}

static XCamReturn
pre_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = XCAM_RETURN_NO_ERROR;

    AdrcHandle_t pAdrcCtx = inparams->ctx->AdrcInstConfig.hAdrc;
    RkAiqAlgoConfigAdrcInt* AdrcCfgParam = (RkAiqAlgoConfigAdrcInt*)inparams;
    const CamCalibDbContext_t* pCalibDb = AdrcCfgParam->rk_com.u.prepare.calib;

    // sence mode
    if (AdrcCfgParam->rk_com.u.proc.gray_mode)
        pAdrcCtx->sence_mode = ADRC_NIGHT;
    else if (pAdrcCtx->FrameNumber == 1)
        pAdrcCtx->sence_mode = ADRC_NORMAL;
    else
        pAdrcCtx->sence_mode = ADRC_HDR;

	pAdrcCtx->sence_mode = 0;
	pAdrcCtx->drcAttr.opMode =DRC_OPMODE_API_OFF;
    LOGD_AHDR("%s:Current mode:%d\n", __FUNCTION__, pAdrcCtx->sence_mode);
    if(pAdrcCtx->drcAttr.opMode == DRC_OPMODE_TOOL)
        AdrcSelectMode(pAdrcCtx, &pAdrcCtx->drcAttr.stTool, pAdrcCtx->sence_mode);
    else
        AdrcSelectMode(pAdrcCtx, &pAdrcCtx->pCalibDB, pAdrcCtx->sence_mode);

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn
processing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    XCamReturn result = XCAM_RETURN_NO_ERROR;
	

    AdrcHandle_t pAdrcCtx = (AdrcHandle_t)inparams->ctx->AdrcInstConfig.hAdrc;
    RkAiqAlgoProcAdrcInt* AdrcParams = (RkAiqAlgoProcAdrcInt*)inparams;
    RkAiqAlgoProcResAdrcInt* AdrcProcResParams = (RkAiqAlgoProcResAdrcInt*)outparams;
    // pAdrcCtx->frameCnt = inparams->frame_id;
    AdrcGetStats(pAdrcCtx, &AdrcParams->ispAdrcStats);
	
    RkAiqAlgoProcResAeInt* ae_proc_res_int =
        (RkAiqAlgoProcResAeInt*)(AdrcParams->rk_com.u.proc.proc_res_comb->ae_proc_res);

    if (ae_proc_res_int)
        AdrcGetSensorInfo(pAdrcCtx, ae_proc_res_int->ae_proc_res_rk);
    else {
        AecProcResult_t AeProcResult;
        LOGW_AHDR("%s: Ae Proc result is null!!!\n", __FUNCTION__);
        AdrcGetSensorInfo(pAdrcCtx, AeProcResult);
    }
	

    RkAiqAlgoPreResAeInt* ae_pre_res_int =
        (RkAiqAlgoPreResAeInt*)(AdrcParams->rk_com.u.proc.pre_res_comb->ae_pre_res);
    RkAiqAlgoPreResAfInt* af_pre_res_int =
        (RkAiqAlgoPreResAfInt*)(AdrcParams->rk_com.u.proc.pre_res_comb->af_pre_res);
    if (ae_pre_res_int && af_pre_res_int)
        AdrcUpdateConfig(pAdrcCtx,
                         ae_pre_res_int->ae_pre_res_rk,
                         af_pre_res_int->af_pre_result);
    else if (ae_pre_res_int) {
        af_preprocess_result_t AfPreResult;
        LOGW_AHDR("%s: af Pre result is null!!!\n", __FUNCTION__);
        AdrcUpdateConfig(pAdrcCtx,
                         ae_pre_res_int->ae_pre_res_rk,
                         AfPreResult);
    }
    else {
        AecPreResult_t AecHdrPreResult;
        af_preprocess_result_t AfPreResult;
        LOGW_AHDR("%s: ae/af Pre result is null!!!\n", __FUNCTION__);
        AdrcUpdateConfig(pAdrcCtx,
                         AecHdrPreResult,
                         AfPreResult);
    }
	
    AdrcProcessing(pAdrcCtx);

    pAdrcCtx->AdrcProcRes.LongFrameMode = pAdrcCtx->SensorInfo.LongFrmMode;
    AdrcProcResParams->AdrcProcRes.isHdrGlobalTmo = pAdrcCtx->AdrcProcRes.isHdrGlobalTmo;
	if(pAdrcCtx->FrameNumber == 2)
		pAdrcCtx->AdrcProcRes.bTmoEn = true;
	else
		pAdrcCtx->AdrcProcRes.bTmoEn = false;
    AdrcProcResParams->AdrcProcRes.bTmoEn = pAdrcCtx->AdrcProcRes.bTmoEn;
    AdrcProcResParams->AdrcProcRes.isLinearTmo = pAdrcCtx->AdrcProcRes.isLinearTmo;
    memcpy(&AdrcProcResParams->AdrcProcRes.DrcProcRes, &pAdrcCtx->AdrcProcRes.DrcProcRes, sizeof(DrcProcRes_t));

	LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn
post_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

RkAiqAlgoDescription g_RkIspAlgoDescAdrc = {
    .common = {
        .version = RKISP_ALGO_ADRC_VERSION,
        .vendor  = RKISP_ALGO_ADRC_VENDOR,
        .description = RKISP_ALGO_ADRC_DESCRIPTION,
        .type    = RK_AIQ_ALGO_TYPE_ADRC,
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
