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

#ifndef _CAM_HW_ISP20_PARAMS_H_
#define _CAM_HW_ISP20_PARAMS_H_

#include "rk_aiq_pool.h"
#include "rkisp2-config.h"
#include "rkispp-config.h"

namespace RkCam {

#define ISP20PARAM_SUBM (0x2)

class Isp20Params {
public:
    explicit Isp20Params() : _last_pp_module_init_ens(0)
        , _force_isp_module_ens(0)
        , _force_ispp_module_ens(0)
        , _force_module_flags(0) {};
    virtual ~Isp20Params() {};

    XCamReturn checkIsp20Params(struct isp2x_isp_params_cfg& isp_cfg);
    XCamReturn convertAiqResultsToIsp20Params(struct isp2x_isp_params_cfg& isp_cfg,
            SmartPtr<RkAiqIspParamsProxy> aiq_results,
            SmartPtr<RkAiqIspParamsProxy>& last_aiq_results);
    XCamReturn convertAiqResultsToIsp20PpParams(struct rkispp_params_cfg& pp_cfg,
            SmartPtr<RkAiqIsppParamsProxy> aiq_results);
    void set_working_mode(int mode);
    void setModuleStatus(rk_aiq_module_id_t mId, bool en);
    void getModuleStatus(rk_aiq_module_id_t mId, bool& en);
    void forceOverwriteAiqIsppCfg(struct rkispp_params_cfg& pp_cfg, SmartPtr<RkAiqIsppParamsProxy> aiq_results);
    void forceOverwriteAiqIspCfg(struct isp2x_isp_params_cfg& isp_cfg, SmartPtr<RkAiqIspParamsProxy> aiq_results);
protected:
    XCAM_DEAD_COPY(Isp20Params);

    template<class T>
    void convertAiqAeToIsp20Params(T& isp_cfg,
                                   const rk_aiq_isp_aec_meas_t& aec_meas);
    template<class T>
    void convertAiqHistToIsp20Params(T& isp_cfg,
                                     const rk_aiq_isp_hist_meas_t& hist_meas);
    template<class T>
    void convertAiqAwbToIsp20Params(T& isp_cfg,
                                    const rk_aiq_awb_stat_cfg_v200_t& awb_meas,
                                    bool awb_cfg_udpate);
    template<class T>
    void convertAiqAwbGainToIsp20Params(T& isp_cfg,
                                        const rk_aiq_wb_gain_t& awb_gain, const rk_aiq_isp_blc_t &blc,
                                        bool awb_gain_update);
    template<class T>
    void convertAiqMergeToIsp20Params(T& isp_cfg,
                                      const rk_aiq_isp_hdr_t& ahdr_data);
    template<class T>
    void convertAiqTmoToIsp20Params(T& isp_cfg,
                                    const rk_aiq_isp_hdr_t& ahdr_data); 
    template<class T>
    void convertAiqAdehazeToIsp20Params(T& isp_cfg,
                                        const rk_aiq_isp_dehaze_t& dhaze                     );
    template<class T>
    void convertAiqAgammaToIsp20Params(T& isp_cfg,
                                       const AgammaProcRes_t& gamma_out_cfg);
    template<class T>
    void convertAiqAdemosaicToIsp20Params(T& isp_cfg,
                                          SmartPtr<RkAiqIspParamsProxy> aiq_results);
    template<class T>
    void convertAiqLscToIsp20Params(T& isp_cfg,
                                    const rk_aiq_lsc_cfg_t& lsc);
    template<class T>
    void convertAiqBlcToIsp20Params(T& isp_cfg,
                                    SmartPtr<RkAiqIspParamsProxy> aiq_results);
    template<class T>
    void convertAiqDpccToIsp20Params(T& isp_cfg,
                                     SmartPtr<RkAiqIspParamsProxy> aiq_results);
    template<class T>
    void convertAiqCcmToIsp20Params(T& isp_cfg,
                                    const rk_aiq_ccm_cfg_t& ccm);
    template<class T>
    void convertAiqA3dlutToIsp20Params(T& isp_cfg,
                                       const rk_aiq_lut3d_cfg_t& lut3d_cfg);
    template<class T>
    void convertAiqCpToIsp20Params(T& isp_cfg,
                                   const rk_aiq_acp_params_t& lut3d_cfg);
    template<class T>
    void convertAiqIeToIsp20Params(T& isp_cfg,
                                   const rk_aiq_isp_ie_t& ie_cfg);
    template<class T>
    void convertAiqRawnrToIsp20Params(T& isp_cfg,
                                      rk_aiq_isp_rawnr_t& rawnr);
    void convertAiqTnrToIsp20Params(struct rkispp_params_cfg& pp_cfg,
                                    rk_aiq_isp_tnr_t& tnr);
    void convertAiqUvnrToIsp20Params(struct rkispp_params_cfg& pp_cfg,
                                     rk_aiq_isp_uvnr_t& uvnr);
    void convertAiqYnrToIsp20Params(struct rkispp_params_cfg& pp_cfg,
                                    rk_aiq_isp_ynr_t& ynr);
    void convertAiqSharpenToIsp20Params(struct rkispp_params_cfg& pp_cfg,
                                        rk_aiq_isp_sharpen_t& sharp, rk_aiq_isp_edgeflt_t& edgeflt);
    template<class T>
    void convertAiqAfToIsp20Params(T& isp_cfg,
                                   const rk_aiq_isp_af_meas_t& af_data, bool af_cfg_udpate);
    template<class T>
    void convertAiqGainToIsp20Params(T& isp_cfg,
                                     rk_aiq_isp_gain_t& gain);
    template<class T>
    void convertAiqAldchToIsp20Params(T& isp_cfg,
                                      const rk_aiq_isp_ldch_t& ldch_cfg);
    void convertAiqFecToIsp20Params(struct rkispp_params_cfg& pp_cfg,
                                    rk_aiq_isp_fec_t& fec);
    template<class T>
    void convertAiqGicToIsp20Params(T& isp_cfg,
                                    const rk_aiq_isp_gic_t& gic_cfg);
    void convertAiqOrbToIsp20Params(struct rkispp_params_cfg& pp_cfg,
                                    rk_aiq_isp_orb_t& orb);
    bool getModuleForceFlag(int module_id);
    void setModuleForceFlagInverse(int module_id);
    bool getModuleForceEn(int module_id);
    void updateIspModuleForceEns(u64 module_ens);
    void updateIsppModuleForceEns(u32 module_ens);
    uint32_t _last_pp_module_init_ens;
    u64 _force_isp_module_ens;
    u32 _force_ispp_module_ens;
    u64 _force_module_flags;
    int _working_mode;
    Mutex _mutex;
};
};
#endif
