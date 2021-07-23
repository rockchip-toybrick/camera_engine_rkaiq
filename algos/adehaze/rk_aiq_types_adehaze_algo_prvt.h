/*
 *rk_aiq_types_adehaze_algo_prvt.h
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

#ifndef _RK_AIQ_TYPES_ADEHAZE_ALGO_PRVT_H_
#define _RK_AIQ_TYPES_ADEHAZE_ALGO_PRVT_H_

#include "adehaze/rk_aiq_types_adehaze_algo_int.h"
#include "RkAiqCalibDbTypes.h"
#include "rk_aiq_types_adehaze_stat.h"

#include "rk_aiq_algo_types.h"
#include "xcam_log.h"




#define LIMIT_VALUE(value,max_value,min_value)      (value > max_value? max_value : value < min_value ? min_value : value)
#define DEHAZEBIGMODE     (2560)
#define RK_DEHAZE_ISO_NUM 9
#define FUNCTION_ENABLE 1
#define FUNCTION_DISABLE 0


typedef struct AdehazeHandle_s {
    CalibDb_Dehaze_t calib_dehaz;
    CamCalibDbContext_t* pCalibDb;
    adehaze_sw_t AdehazeAtrr;
    RkAiqAdehazeProcResult_t ProcRes;
    rkisp_adehaze_stats_t stats;
    int HWversion; //0:isp2.0 1:isp2.1
    int width;
    int height;
    int strength;
    int working_mode;
    int Dehaze_Scene_mode;
    int FrameNumber;
} AdehazeHandle_t;


#endif

