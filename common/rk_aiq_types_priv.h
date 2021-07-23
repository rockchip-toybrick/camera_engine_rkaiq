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

#ifndef _RK_AIQ_TYPES_PRIV_H_
#define _RK_AIQ_TYPES_PRIV_H_

#include "rk_aiq_types.h"

#define RKAIQ_ISP_LDCH_ID           (1 << 0)

// base struct
typedef struct {
    uint32_t update_mask;
    uint32_t module_enable_mask;
    sint32_t frame_id;
} rk_aiq_isp_params_t;

// v2x params struct
typedef struct : rk_aiq_isp_params_t {
    rk_aiq_isp_aec_meas_t   aec_meas;
    rk_aiq_isp_hist_meas_t  hist_meas;
    bool awb_cfg_update;
    bool awb_gain_update;
    rk_aiq_wb_gain_t        awb_gain;
    bool af_cfg_update;
    rk_aiq_isp_af_meas_t    af_meas;
    rk_aiq_isp_dpcc_t       dpcc;
    rk_aiq_isp_merge_t      merge;
    rk_aiq_lsc_cfg_t        lsc;
    rk_aiq_isp_demosaic_t   demosaic;
    rk_aiq_isp_ldch_t       ldch;
    rk_aiq_lut3d_cfg_t      lut3d;
    AgammaProcRes_t         agamma;
    rk_aiq_isp_wdr_t        wdr;
    rk_aiq_isp_csm_t        csm;
    rk_aiq_isp_cgc_t        cgc;
    rk_aiq_isp_conv422_t    conv22;
    rk_aiq_isp_yuvconv_t    yuvconv;
    rk_aiq_isp_gain_t       gain_config;
    rk_aiq_acp_params_t     cp;
    rk_aiq_isp_ie_t         ie;
#ifdef RK_SIMULATOR_HW
    rkaiq_anr_procRes_t     rkaiq_anr_proc_res;
    rkaiq_asharp_procRes_t  rkaiq_asharp_proc_res;
#endif
    // TODO: should be split to merge and tmo
    RkAiqAhdrProcResult_t      ahdr_proc_res;//porc data for hw/simulator
} rk_aiq_isp_params_v2x_t;

// v20 params struct
typedef struct : rk_aiq_isp_params_v2x_t {
    rk_aiq_isp_blc_t           blc;
    rk_aiq_isp_dehaze_t        adhaz;
    rk_aiq_ccm_cfg_t           ccm;
    rk_aiq_isp_gic_t           gic;
    // TMO
    rk_aiq_isp_drc_t           drc;
    rk_aiq_isp_rawnr_t         rawnr;
    rk_aiq_awb_stat_cfg_v200_t awb_cfg;
} rk_aiq_isp_params_v20_t;

// v21 params struct
typedef struct : rk_aiq_isp_params_v2x_t {
    rk_aiq_isp_blc_v21_t        blc;
    rk_aiq_isp_dehaze_v21_t   adhaz;
    rk_aiq_ccm_cfg_t        ccm;
    rk_aiq_isp_gic_v21_t        gic;
    // TMO
    rk_aiq_isp_drc_v21_t        drc;
    rk_aiq_awb_stat_cfg_v201_t  awb_cfg;
    rk_aiq_isp_baynr_v21_t      rawnr;
    rk_aiq_isp_bay3d_v21_t      tnr;
    rk_aiq_isp_ynr_v21_t        ynr;
    rk_aiq_isp_cnr_v21_t        uvnr;
    rk_aiq_isp_sharp_v21_t      sharp;
} rk_aiq_isp_params_v21_t;

#define RKAIQ_ISPP_TNR_ID           (1 << 0)
#define RKAIQ_ISPP_NR_ID            (1 << 1)
#define RKAIQ_ISPP_SHARP_ID         (1 << 2)
#define RKAIQ_ISPP_FEC_ID           (1 << 3)
#define RKAIQ_ISPP_ORB_ID           (1 << 4)

typedef struct {
    uint32_t update_mask;
    sint32_t frame_id;
    rk_aiq_isp_tnr_t        tnr;
    rk_aiq_isp_ynr_t        ynr;
    rk_aiq_isp_uvnr_t       uvnr;
    rk_aiq_isp_sharpen_t    sharpen;
    rk_aiq_isp_edgeflt_t    edgeflt;
    rk_aiq_isp_orb_t        orb;
    rk_aiq_isp_fec_t        fec;
} rk_aiq_ispp_params_t;

typedef enum rk_aiq_drv_share_mem_type_e {
    MEM_TYPE_LDCH,
    MEM_TYPE_FEC,
} rk_aiq_drv_share_mem_type_t;

typedef void (*alloc_mem_t)(void* cfg, void* ops_ctx, void** mem_ctx);
typedef void (*release_mem_t)(void* mem_ctx);
typedef void* (*get_free_item_t)(void* mem_ctx);
typedef struct isp_drv_share_mem_ops_s {
    alloc_mem_t alloc_mem;
    release_mem_t release_mem;
    get_free_item_t get_free_item;
} isp_drv_share_mem_ops_t;

typedef struct rk_aiq_ldch_share_mem_info_s {
    int size;
    void *map_addr;
    void *addr;
    int fd;
    char *state;
} rk_aiq_ldch_share_mem_info_t;

typedef struct rk_aiq_fec_share_mem_info_s {
    int size;
    int fd;
    void *map_addr;
    unsigned char *meshxf;
    unsigned char *meshyf;
    unsigned short *meshxi;
    unsigned short *meshyi;
    char *state;
} rk_aiq_fec_share_mem_info_t;

typedef struct rk_aiq_share_mem_alloc_param_s {
    int width;
    int height;
    char reserved[8];
} rk_aiq_share_mem_alloc_param_t;

typedef struct rk_aiq_share_mem_config_s {
    rk_aiq_drv_share_mem_type_t mem_type;
    rk_aiq_share_mem_alloc_param_t alloc_param;
} rk_aiq_share_mem_config_t;

#endif
