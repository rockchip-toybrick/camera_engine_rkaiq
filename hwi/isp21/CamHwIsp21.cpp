/*
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

#include "CamHwIsp21.h"
#include "isp20/Isp20PollThread.h"
#ifdef ANDROID_OS
#include <cutils/properties.h>
#endif

namespace RkCam {

CamHwIsp21::CamHwIsp21()
    : CamHwIsp20()
{
    mNoReadBack = true;
#ifndef ANDROID_OS
    char* valueStr = getenv("normal_no_read_back");
    if (valueStr) {
        mNoReadBack = atoi(valueStr) > 0 ? true : false;
    }
#else
    char property_value[PROPERTY_VALUE_MAX] = {'\0'};

    property_get("persist.vendor.rkisp_no_read_back", property_value, "-1");
    int val = atoi(property_value);
    if (val != -1)
        mNoReadBack = atoi(property_value) > 0 ? true : false;
#endif
}

CamHwIsp21::~CamHwIsp21()
{}

XCamReturn
CamHwIsp21::init(const char* sns_ent_name)
{
    xcam_mem_clear (_full_active_isp21_params);

    XCamReturn ret = CamHwIsp20::init(sns_ent_name);

    SmartPtr<Isp20PollThread> isp20Pollthread =
        mPollthread.dynamic_cast_ptr<Isp20PollThread>();
    isp20Pollthread->set_need_luma_rd_info(false);
    return ret;
}

void
CamHwIsp21::gen_full_isp_params(const struct isp21_isp_params_cfg* update_params,
                                struct isp21_isp_params_cfg* full_params)
{
    XCAM_ASSERT (update_params);
    XCAM_ASSERT (full_params);
    int i = 0;

    ENTER_CAMHW_FUNCTION();
    for (; i <= RK_ISP2X_MAX_ID; i++)
        if (update_params->module_en_update & (1LL << i)) {
            full_params->module_en_update |= 1LL << i;
            // clear old bit value
            full_params->module_ens &= ~(1LL << i);
            // set new bit value
            full_params->module_ens |= update_params->module_ens & (1LL << i);
        }

    for (i = 0; i <= RK_ISP2X_MAX_ID; i++) {
        if (update_params->module_cfg_update & (1LL << i)) {
            full_params->module_cfg_update |= 1LL << i;
            switch (i) {
            case RK_ISP2X_RAWAE_0_ID:
                full_params->meas.rawae0 = update_params->meas.rawae0;
                break;
            case RK_ISP2X_RAWAE_1_ID:
                full_params->meas.rawae1 = update_params->meas.rawae1;
                break;
            case RK_ISP2X_RAWAE_2_ID:
                full_params->meas.rawae2 = update_params->meas.rawae2;
                break;
            case RK_ISP2X_RAWAE_3_ID:
                full_params->meas.rawae3 = update_params->meas.rawae3;
                break;
            case RK_ISP2X_RAWHIST_0_ID:
                full_params->meas.rawhist0 = update_params->meas.rawhist0;
                break;
            case RK_ISP2X_RAWHIST_1_ID:
                full_params->meas.rawhist1 = update_params->meas.rawhist1;
                break;
            case RK_ISP2X_RAWHIST_2_ID:
                full_params->meas.rawhist2 = update_params->meas.rawhist2;
                break;
            case RK_ISP2X_RAWHIST_3_ID:
                full_params->meas.rawhist3 = update_params->meas.rawhist3;
                break;
            case RK_ISP2X_YUVAE_ID:
                full_params->meas.yuvae = update_params->meas.yuvae;
                break;
            case RK_ISP2X_SIHST_ID:
                full_params->meas.sihst = update_params->meas.sihst;
                break;
            case RK_ISP2X_SIAWB_ID:
                full_params->meas.siawb = update_params->meas.siawb;
                break;
            case RK_ISP2X_RAWAWB_ID:
                full_params->meas.rawawb = update_params->meas.rawawb;
                break;
            case RK_ISP2X_AWB_GAIN_ID:
                full_params->others.awb_gain_cfg = update_params->others.awb_gain_cfg;
                break;
            case RK_ISP2X_RAWAF_ID:
                full_params->meas.rawaf = update_params->meas.rawaf;
                break;
            case RK_ISP2X_HDRMGE_ID:
                full_params->others.hdrmge_cfg = update_params->others.hdrmge_cfg;
                break;
            /* case RK_ISP2X_HDRTMO_ID: */
            /*     full_params->others.hdrtmo_cfg = update_params->others.hdrtmo_cfg; */
            /*     break; */
            case RK_ISP2X_CTK_ID:
                full_params->others.ccm_cfg = update_params->others.ccm_cfg;
                break;
            case RK_ISP2X_LSC_ID:
                full_params->others.lsc_cfg = update_params->others.lsc_cfg;
                break;
            case RK_ISP2X_GOC_ID:
                full_params->others.gammaout_cfg = update_params->others.gammaout_cfg;
                break;
            case RK_ISP2X_3DLUT_ID:
                full_params->others.isp3dlut_cfg = update_params->others.isp3dlut_cfg;
                break;
            case RK_ISP2X_DPCC_ID:
                full_params->others.dpcc_cfg = update_params->others.dpcc_cfg;
                break;
            case RK_ISP2X_BLS_ID:
                full_params->others.bls_cfg = update_params->others.bls_cfg;
                break;
            case RK_ISP2X_DEBAYER_ID:
                full_params->others.debayer_cfg = update_params->others.debayer_cfg;
                break;
            case RK_ISP2X_DHAZ_ID:
                full_params->others.dhaz_cfg = update_params->others.dhaz_cfg;
                break;
            /* case RK_ISP2X_RAWNR_ID: */
            /*     full_params->others.rawnr_cfg = update_params->others.rawnr_cfg; */
            /*     break; */
            /* case RK_ISP2X_GAIN_ID: */
            /*     full_params->others.gain_cfg = update_params->others.gain_cfg; */
            /*     break; */
            case RK_ISP2X_LDCH_ID:
                full_params->others.ldch_cfg = update_params->others.ldch_cfg;
                break;
            case RK_ISP2X_GIC_ID:
                full_params->others.gic_cfg = update_params->others.gic_cfg;
                break;
            case RK_ISP2X_CPROC_ID:
                full_params->others.cproc_cfg = update_params->others.cproc_cfg;
                break;
            case Rk_ISP21_BAYNR_ID:
                full_params->others.baynr_cfg = update_params->others.baynr_cfg;
                break;
            case Rk_ISP21_BAY3D_ID:
                full_params->others.bay3d_cfg = update_params->others.bay3d_cfg;
                break;
            case Rk_ISP21_YNR_ID:
                full_params->others.ynr_cfg = update_params->others.ynr_cfg;
                break;
            case Rk_ISP21_CNR_ID:
                full_params->others.cnr_cfg = update_params->others.cnr_cfg;
                break;
            case Rk_ISP21_SHARP_ID:
                full_params->others.sharp_cfg = update_params->others.sharp_cfg;
                break;
            case Rk_ISP21_DRC_ID:
                full_params->others.drc_cfg = update_params->others.drc_cfg;
                break;
            default:
                break;
            }
        }
    }
    EXIT_CAMHW_FUNCTION();
}


/*
 * some module(HDR/TNR) parameters are related to the next frame exposure
 * and can only be easily obtained at the hwi layer,
 * so these parameters are calculated at hwi and the result is overwritten.
 */
XCamReturn
CamHwIsp21::overrideExpRatioV21ToAiqResults(const sint32_t frameId,
        int module_id,
        SmartPtr<RkAiqIspParamsProxy>& aiq_results)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    SmartPtr<RkAiqExpParamsProxy> curFrameExpParam;
    SmartPtr<RkAiqExpParamsProxy> nextFrameExpParam;
    SmartPtr<SensorHw> mSensorSubdev = mSensorDev.dynamic_cast_ptr<SensorHw>();
    rk_aiq_isp_params_v21_t* isp21_result =
        static_cast<rk_aiq_isp_params_v21_t*>(aiq_results->data().ptr());

    if (mSensorSubdev.ptr()) {
        ret = mSensorSubdev->getEffectiveExpParams(curFrameExpParam, frameId);
        if (ret != XCAM_RETURN_NO_ERROR) {
            LOGE_CAMHW_SUBM(ISP20HW_SUBM, "exp-sync: module_id: 0x%x, rx id: %d, ispparams id: %d\n",
                            module_id,
                            frameId,
                            isp21_result->frame_id);
            return ret;
        }

        ret = mSensorSubdev->getEffectiveExpParams(nextFrameExpParam, frameId + 1);
        if (ret != XCAM_RETURN_NO_ERROR) {
            LOGE_CAMHW_SUBM(ISP20HW_SUBM, "exp-sync: module_id: 0x%x, rx id: %d, ispparams id: %d\n",
                            module_id,
                            frameId + 1,
                            isp21_result->frame_id);
            return ret;
        }
    }

    LOGD_CAMHW_SUBM(ISP20HW_SUBM, "exp-sync: module_id: 0x%x, rx id: %d, ispparams id: %d\n"
                    "curFrame: lexp: %f-%f, mexp: %f-%f, sexp: %f-%f\n"
                    "nextFrame: lexp: %f-%f, mexp: %f-%f, sexp: %f-%f\n",
                    module_id,
                    frameId,
                    isp21_result->frame_id,
                    curFrameExpParam->data()->aecExpInfo.HdrExp[2].exp_real_params.analog_gain,
                    curFrameExpParam->data()->aecExpInfo.HdrExp[2].exp_real_params.integration_time,
                    curFrameExpParam->data()->aecExpInfo.HdrExp[1].exp_real_params.analog_gain,
                    curFrameExpParam->data()->aecExpInfo.HdrExp[1].exp_real_params.integration_time,
                    curFrameExpParam->data()->aecExpInfo.HdrExp[0].exp_real_params.analog_gain,
                    curFrameExpParam->data()->aecExpInfo.HdrExp[0].exp_real_params.integration_time,
                    nextFrameExpParam->data()->aecExpInfo.HdrExp[2].exp_real_params.analog_gain,
                    nextFrameExpParam->data()->aecExpInfo.HdrExp[2].exp_real_params.integration_time,
                    nextFrameExpParam->data()->aecExpInfo.HdrExp[1].exp_real_params.analog_gain,
                    nextFrameExpParam->data()->aecExpInfo.HdrExp[1].exp_real_params.integration_time,
                    nextFrameExpParam->data()->aecExpInfo.HdrExp[0].exp_real_params.analog_gain,
                    nextFrameExpParam->data()->aecExpInfo.HdrExp[0].exp_real_params.integration_time);

    switch (module_id) {
    case Rk_ISP21_DRC_ID:
    {
        float nextLExpo = 0;
        float nextMExpo = 0;
        float nextSExpo = 0;
        if (1) {
            nextLExpo = nextFrameExpParam->data()->aecExpInfo.HdrExp[1].exp_real_params.analog_gain * \
                        nextFrameExpParam->data()->aecExpInfo.HdrExp[1].exp_real_params.integration_time;
            nextMExpo = nextLExpo;
            nextSExpo = nextFrameExpParam->data()->aecExpInfo.HdrExp[0].exp_real_params.analog_gain * \
                        nextFrameExpParam->data()->aecExpInfo.HdrExp[0].exp_real_params.integration_time;
        }
        float nextRatioLS = nextLExpo / nextSExpo;

        //get sw_drc_compres_scl
        float MFHDR_LOG_Q_BITS = 11;
        float adrc_gain = (float)(isp21_result->drc.DrcProcRes.sw_drc_adrc_gain);
        float log_ratio2 = log(nextRatioLS * adrc_gain / 1024) / log(2.0f) + 12;
        float offsetbits_int = (float)(isp21_result->drc.DrcProcRes.sw_drc_offset_pow2);
        float offsetbits = offsetbits_int * pow(2, MFHDR_LOG_Q_BITS);
        float hdrbits = log_ratio2 * pow(2, MFHDR_LOG_Q_BITS);
        float hdrvalidbits = hdrbits - offsetbits;
        float compres_scl = (12 * pow(2, MFHDR_LOG_Q_BITS * 2)) / hdrvalidbits;
        isp21_result->drc.DrcProcRes.sw_drc_compres_scl = (int)(compres_scl);

        //get sw_drc_min_ogain
        float sw_drc_min_ogain = 1 / (nextRatioLS * adrc_gain / 1024);
        isp21_result->drc.DrcProcRes.sw_drc_min_ogain = (int)(sw_drc_min_ogain * pow(2, 15) + 0.5);

        //get sw_drc_compres_y
        float curveparam, curveparam2, curveparam3, tmp;
        float luma2[17] = { 0, 1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 10240, 12288, 14336, 16384, 18432, 20480, 22528, 24576 };
        float curveTable[17];
        float ISP_RAW_BIT = 12;
        float dstbits = ISP_RAW_BIT * pow(2, MFHDR_LOG_Q_BITS);
        float validbits = dstbits - offsetbits;

        for(int i = 0; i < ISP21_DRC_Y_NUM; ++i)
        {

            curveparam = (float)(validbits - 0) / (hdrvalidbits - validbits);
            curveparam2 = validbits * (1 + curveparam);
            curveparam3 = hdrvalidbits * curveparam;
            tmp = luma2[i] * hdrvalidbits / 24576;
            curveTable[i] = (tmp * curveparam2 / (tmp + curveparam3));
            isp21_result->drc.DrcProcRes.sw_drc_compres_y[i] = (int)(curveTable[i]) ;
        }

        LOGD_CAMHW_SUBM(ISP20HW_SUBM, "nextRatioLS:%f sw_drc_compres_scl:%d sw_drc_min_ogain:%d\n",
                        nextRatioLS, isp21_result->drc.DrcProcRes.sw_drc_compres_scl, isp21_result->drc.DrcProcRes.sw_drc_min_ogain);
        for(int i = 0; i < ISP21_DRC_Y_NUM; ++i)
            LOGD_CAMHW_SUBM(ISP20HW_SUBM, "sw_drc_compres_y[%d]:%d\n", i, isp21_result->drc.DrcProcRes.sw_drc_compres_y[i]);

        break;
    }
    default:
        LOGW_CAMHW_SUBM(ISP20HW_SUBM, "unkown module id: 0x%x!\n", module_id);
        break;
    }

    return ret;
}


XCamReturn
CamHwIsp21::setIspParamsSync(int frameId)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ENTER_CAMHW_FUNCTION();
    CamHwIsp20::_mutex.lock();
    while (_effecting_ispparm_map.size() > 4)
        _effecting_ispparm_map.erase(_effecting_ispparm_map.begin());

    if (_pending_ispparams_queue.empty()) {
        LOGW_CAMHW_SUBM(ISP20HW_SUBM, "no new isp params for frame %d !", frameId);
        CamHwIsp20::_mutex.unlock();

        return ret;
    }
    // merge all pending params
    struct isp21_isp_params_cfg update_params;
    SmartPtr<RkAiqIspParamsProxy> aiq_results;
    rk_aiq_isp_params_v21_t* isp21_result;

    /* xcam_mem_clear (update_params); */
    /* xcam_mem_clear (_full_active_isp_params); */
    update_params.module_en_update = 0;
    update_params.module_ens = 0;
    update_params.module_cfg_update = 0;
    if (_state == CAM_HW_STATE_STOPPED || _state == CAM_HW_STATE_PREPARED || _state == CAM_HW_STATE_PAUSED) {
        // update all ens
        _full_active_isp21_params.module_en_update = ~0;
        // just re-config the enabled moddules
        _full_active_isp21_params.module_cfg_update = _full_active_isp21_params.module_ens;
    } else {
        _full_active_isp21_params.module_en_update = 0;
        // use module_ens to store module status, so we can use it to restore
        // the init params for re-start and re-prepare
        /* _full_active_isp_params.module_ens = 0; */
        _full_active_isp21_params.module_cfg_update = 0;
    }
    LOGD_CAMHW_SUBM(ISP20HW_SUBM, "merge isp params num %d\n", _pending_ispparams_queue.size());
    aiq_results = _pending_ispparams_queue.back();
    isp21_result = static_cast<rk_aiq_isp_params_v21_t*>(aiq_results->data().ptr());
    if (isp21_result->frame_id !=  frameId) {
        if (_last_aiq_results->data()->frame_id > isp21_result->frame_id)
            aiq_results = _last_aiq_results;
        isp21_result = static_cast<rk_aiq_isp_params_v21_t*>(aiq_results->data().ptr());
        LOGW_CAMHW_SUBM(ISP20HW_SUBM, "isp stream sequence(%d) != aiq params id(%d), last params is id(%d)\n",
                        frameId, isp21_result ->frame_id, _last_aiq_results->data()->frame_id);
    } else {
        _pending_ispparams_queue.pop_back();
    }

    CamHwIsp20::_mutex.unlock();

    // TODO : need this ?
#if 1
    if (RK_AIQ_HDR_GET_WORKING_MODE(_hdr_mode) != RK_AIQ_WORKING_MODE_NORMAL) {
        ret = overrideExpRatioToAiqResults(frameId, RK_ISP2X_HDRTMO_ID, aiq_results);
        ret = overrideExpRatioV21ToAiqResults(frameId, Rk_ISP21_DRC_ID, aiq_results);
        if (ret != XCAM_RETURN_NO_ERROR) {
            LOGE_CAMHW_SUBM(ISP20HW_SUBM, "convertExpRatioToAiqResults error!\n");
        }
    }
#endif
    ret = convertAiqResultsToIsp21Params(update_params, aiq_results, _last_aiq_results);
    if (ret != XCAM_RETURN_NO_ERROR) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "rkisp1_convert_results error\n");
    }
    // TODO
#if 0
    forceOverwriteAiqIspCfg(update_params, aiq_results);
#endif
    gen_full_isp_params(&update_params, &_full_active_isp21_params);

    if (_state == CAM_HW_STATE_STOPPED)
        LOGD_CAMHW_SUBM(ISP20HW_SUBM, "ispparam ens 0x%llx, en_up 0x%llx, cfg_up 0x%llx",
                        _full_active_isp21_params.module_ens,
                        _full_active_isp21_params.module_en_update,
                        _full_active_isp21_params.module_cfg_update);

#ifdef RUNTIME_MODULE_DEBUG
    _full_active_isp21_params.module_en_update &= ~g_disable_isp_modules_en;
    _full_active_isp21_params.module_ens |= g_disable_isp_modules_en;
    _full_active_isp21_params.module_cfg_update &= ~g_disable_isp_modules_cfg_update;
#endif
    // TODO
    /* dump_isp_config(&_full_active_isp21_params, aiq_results); */

    if (mIspParamsDev.ptr()) {
        struct isp21_isp_params_cfg* isp_params;
        SmartPtr<V4l2Buffer> v4l2buf;

        ret = mIspParamsDev->get_buffer(v4l2buf);
        if (!ret) {
            int buf_index = v4l2buf->get_buf().index;

            isp_params = (struct isp21_isp_params_cfg*)v4l2buf->get_buf().m.userptr;
            *isp_params = _full_active_isp21_params;
            isp_params->frame_id = frameId;

            if (mIspParamsDev->queue_buffer (v4l2buf) != 0) {
                LOGE_CAMHW_SUBM(ISP20HW_SUBM, "RKISP1: failed to ioctl VIDIOC_QBUF for index %d, %d %s.\n",
                                buf_index, errno, strerror(errno));
                return ret;
            }

            CamHwIsp20::_mutex.lock();
            _effecting_ispparm_map[frameId] = aiq_results;
            CamHwIsp20::_mutex.unlock();
            LOGD_CAMHW_SUBM(ISP20HW_SUBM, "ispparam ens 0x%llx, en_up 0x%llx, cfg_up 0x%llx",
                            _full_active_isp21_params.module_ens,
                            _full_active_isp21_params.module_en_update,
                            _full_active_isp21_params.module_cfg_update);

            /* setDhazState(isp21_result->adhaz_config.dehaze_en[0]); */
            LOGD_CAMHW_SUBM(ISP20HW_SUBM, "device(%s) queue buffer index %d, queue cnt %d, check exit status again[exit: %d]",
                            XCAM_STR (mIspParamsDev->get_device_name()),
                            buf_index, mIspParamsDev->get_queued_bufcnt(), _is_exit);
        } else {
            LOGE_CAMHW_SUBM(ISP20HW_SUBM, "Can not get buffer\n");
        }

        if (_is_exit)
            return XCAM_RETURN_BYPASS;
    }

    EXIT_CAMHW_FUNCTION();
    return ret;
}

XCamReturn
CamHwIsp21::setIsppParams(SmartPtr<RkAiqIsppParamsProxy>& isppParams)
{
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
CamHwIsp21::setIsppParamsSync(int frameId)
{
    return XCAM_RETURN_NO_ERROR;
}

void CamHwIsp21::setMulCamConc(bool cc)
{
    mNoReadBack = !cc;
    return CamHwIsp20::setMulCamConc(cc);
}

};
