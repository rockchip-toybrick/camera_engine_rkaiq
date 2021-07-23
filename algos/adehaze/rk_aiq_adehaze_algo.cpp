/*
 * rk_aiq_algo_acprc_itf.c
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
#include <string.h>
#include "rk_aiq_adehaze_algo.h"
#include "xcam_log.h"

#define LIMIT_VALUE(value,max_value,min_value)      (value > max_value? max_value : value < min_value ? min_value : value)

float  LinearInterp(const float *pX, const float *pY, float posx, int XSize)
{
    int index;
    float yOut = 0;

    if (posx >= pX[XSize - 1])
    {
        yOut = pY[XSize - 1];
    }
    else if (posx <= pX[0])
    {
        yOut = pY[0];
    }
    else
    {
        index = 0;
        while((posx >= pX[index]) && (index < XSize))
        {
            index++;
        }
        index -= 1;
        yOut = ((pY[index + 1] - pY[index]) / (pX[index + 1] - pX[index]) * (posx - pX[index]))
               + pY[index];
    }

    return yOut;
}

int LinearInterpEnable(const float *pX, const unsigned char *pY, float posx, int XSize)
{
    int index;
    float out;
    float yOut = 0;
    if (posx >= pX[XSize - 1])
    {
        out = (float)pY[XSize - 1];
    }
    else if (posx <= pX[0])
    {
        out = pY[0];
    }
    else
    {
        index = 0;
        while((posx >= pX[index]) && (index < XSize))
        {
            index++;
        }
        index -= 1;
        out = ((pY[index + 1] - pY[index]) / (pX[index + 1] - pX[index]) * (posx - pX[index]))
              + pY[index];
    }
    yOut = out > 0.5 ? 1 : 0;

    return yOut;

}

void EnableSetting(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    ProcRes->ProcResV20.enable = para->calib_v20[mode].en != 0 ? true : false;

    bool dehaze_enable = false;
    bool enhance_enable = false;
    if(para->calib_v20[mode].dehaze_setting.en != 0 && para->calib_v20[mode].enhance_setting.en != 0)
    {
        ProcRes->ProcResV20.dc_en = 1;
        ProcRes->ProcResV20.enhance_en = 1;
    }
    else if(para->calib_v20[mode].dehaze_setting.en != 0 && para->calib_v20[mode].enhance_setting.en == 0)
    {
        ProcRes->ProcResV20.dc_en = 1;
        ProcRes->ProcResV20.enhance_en = 0;
    }
    else if(para->calib_v20[mode].dehaze_setting.en == 0 && para->calib_v20[mode].enhance_setting.en != 0)
    {
        ProcRes->ProcResV20.dc_en = 1;
        ProcRes->ProcResV20.enhance_en = 1;
    }
    else
    {
        ProcRes->ProcResV20.dc_en = 0;
        ProcRes->ProcResV20.enhance_en = 0;
    }
    dehaze_enable = (ProcRes->ProcResV20.dc_en & 0x1) && (!(ProcRes->ProcResV20.enhance_en & 0x1));
    enhance_enable = (ProcRes->ProcResV20.dc_en & 0x1) && (ProcRes->ProcResV20.enhance_en & 0x1);

    if(para->calib_v20[mode].hist_setting.en != 0)
        ProcRes->ProcResV20.hist_en = 0x1;
    else
        ProcRes->ProcResV20.hist_en = 0;

    LOGD_ADEHAZE(" %s: Dehaze fuction en:%d\n", __func__, ProcRes->ProcResV20.enable);
    LOGD_ADEHAZE(" %s: Dehaze en:%d, Enhance en:%d, Hist en:%d\n", __func__, dehaze_enable, enhance_enable, ProcRes->ProcResV20.hist_en);

    LOG1_ADEHAZE("EIXT: %s \n", __func__);
}

void EnableSettingV21(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    ProcRes->ProcResV21.enable = para->calib_v21[mode].en != 0 ? true : false;

    bool dehaze_enable = false;
    bool enhance_enable = false;
    if(para->calib_v21[mode].dehaze_setting.en != 0 && para->calib_v21[mode].enhance_setting.en != 0)
    {
        ProcRes->ProcResV21.dc_en = 1;
        ProcRes->ProcResV21.enhance_en = 1;
    }
    else if(para->calib_v21[mode].dehaze_setting.en != 0 && para->calib_v21[mode].enhance_setting.en == 0)
    {
        ProcRes->ProcResV21.dc_en = 1;
        ProcRes->ProcResV21.enhance_en = 0;
    }
    else if(para->calib_v21[mode].dehaze_setting.en == 0 && para->calib_v21[mode].enhance_setting.en != 0)
    {
        ProcRes->ProcResV21.dc_en = 1;
        ProcRes->ProcResV21.enhance_en = 1;
    }
    else
    {
        ProcRes->ProcResV21.dc_en = 0;
        ProcRes->ProcResV21.enhance_en = 0;
    }
    dehaze_enable = (ProcRes->ProcResV21.dc_en & 0x1) && (!(ProcRes->ProcResV21.enhance_en & 0x1));
    enhance_enable = (ProcRes->ProcResV21.dc_en & 0x1) && (ProcRes->ProcResV21.enhance_en & 0x1);

    if(para->calib_v21[mode].hist_setting.en != 0)
        ProcRes->ProcResV21.hist_en = 0x1;
    else
        ProcRes->ProcResV21.hist_en = 0;

    LOGD_ADEHAZE(" %s: Dehaze fuction en:%d\n", __func__, ProcRes->ProcResV21.enable);
    LOGD_ADEHAZE(" %s: Dehaze en:%d, Enhance en:%d, Hist en:%d\n", __func__, dehaze_enable, enhance_enable, ProcRes->ProcResV21.hist_en);

    LOG1_ADEHAZE("EIXT: %s \n", __func__);
}

void GetDehazeParams(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    // dehaze_self_adp[7]
    float dc_min_th = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.dc_min_th, iso, RK_DEHAZE_ISO_NUM);
    float dc_max_th = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.dc_max_th, iso, RK_DEHAZE_ISO_NUM);
    float yhist_th = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.yhist_th, iso,  RK_DEHAZE_ISO_NUM);
    float yblk_th = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.yblk_th, iso, RK_DEHAZE_ISO_NUM);
    float dark_th = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.dark_th, iso, RK_DEHAZE_ISO_NUM);
    float bright_min = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.bright_min, iso, RK_DEHAZE_ISO_NUM);
    float bright_max = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.bright_max, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_range_adj[6]
    float wt_max = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.wt_max, iso, RK_DEHAZE_ISO_NUM);
    float air_max = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.air_max, iso, RK_DEHAZE_ISO_NUM);
    float air_min = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.air_min, iso, RK_DEHAZE_ISO_NUM);
    float tmax_base = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.tmax_base, iso, RK_DEHAZE_ISO_NUM);
    float tmax_off = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.tmax_off, iso, RK_DEHAZE_ISO_NUM);
    float tmax_max = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.tmax_max, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_iir_control[5]
    float stab_fnum = para->calib_v20[mode].dehaze_setting.IIR_setting.stab_fnum;
    float sigma = para->calib_v20[mode].dehaze_setting.IIR_setting.sigma;
    float wt_sigma = para->calib_v20[mode].dehaze_setting.IIR_setting.wt_sigma;
    float air_sigma = para->calib_v20[mode].dehaze_setting.IIR_setting.air_sigma;
    float tmax_sigma = para->calib_v20[mode].dehaze_setting.IIR_setting.tmax_sigma;

    float cfg_wt = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.cfg_wt, iso, RK_DEHAZE_ISO_NUM);
    float cfg_air = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.cfg_air, iso, RK_DEHAZE_ISO_NUM);
    float cfg_tmax = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.cfg_tmax, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_bi_para[4]
    float dc_thed = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.dc_thed, iso, RK_DEHAZE_ISO_NUM);
    float dc_weitcur = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.dc_weitcur, iso, RK_DEHAZE_ISO_NUM);
    float air_thed = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.air_thed, iso, RK_DEHAZE_ISO_NUM);
    float air_weitcur = LinearInterp(para->calib_v20[mode].dehaze_setting.iso, para->calib_v20[mode].dehaze_setting.air_weitcur, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_dc_bf_h[25]
    float dc_bf_h[25] = {12.0000, 17.0000, 19.0000, 17.0000, 12.0000,
                         17.0000, 25.0000, 28.0000, 25.0000, 17.0000,
                         19.0000, 28.0000, 32.0000, 28.0000, 19.0000,
                         17.0000, 25.0000, 28.0000, 25.0000, 17.0000,
                         12.0000, 17.0000, 19.0000, 17.0000, 12.0000
                        };

    // dehaze_air_bf_h[9],dehaze_gaus_h[9]
    float air_bf_h[9] = {25.0000, 28.0000, 25.0000,
                         28.0000, 32.0000, 28.0000,
                         25.0000, 28.0000, 25.0000
                        };
    float gaus_h[9] = {2.0000, 4.0000, 2.0000,
                       4.0000, 8.0000, 4.0000,
                       2.0000, 4.0000, 2.0000
                      };

    int rawWidth = 1920;
    int rawHeight = 1080;
    ProcRes->ProcResV20.dc_min_th    = int(dc_min_th); //0~255, (8bit) dc_min_th
    ProcRes->ProcResV20.dc_max_th    = int(dc_max_th);  //0~255, (8bit) dc_max_th
    ProcRes->ProcResV20.yhist_th    = int(yhist_th);  //0~255, (8bit) yhist_th
    ProcRes->ProcResV20.yblk_th    = int(yblk_th * ((rawWidth + 15) / 16) * ((rawHeight + 15) / 16)); //default:28,(9bit) yblk_th
    ProcRes->ProcResV20.dark_th    = int(dark_th);  //0~255, (8bit) dark_th
    ProcRes->ProcResV20.bright_min   = int(bright_min);  //0~255, (8bit) bright_min
    ProcRes->ProcResV20.bright_max   = int(bright_max);  //0~255, (8bit) bright_max
    ProcRes->ProcResV20.wt_max   = int(wt_max * 256); //0~255, (9bit) wt_max
    ProcRes->ProcResV20.air_min   = int(air_min);  //0~255, (8bit) air_min
    ProcRes->ProcResV20.air_max   = int(air_max);  //0~256, (8bit) air_max
    ProcRes->ProcResV20.tmax_base   = int(tmax_base);  //0~255, (8bit) tmax_base
    ProcRes->ProcResV20.tmax_off   = int(tmax_off * 1024); //0~1024,(10bit) tmax_off
    ProcRes->ProcResV20.tmax_max   = int(tmax_max * 1024); //0~1024,(10bit) tmax_max
    ProcRes->ProcResV20.stab_fnum = int(stab_fnum);  //1~31,  (5bit) stab_fnum
    ProcRes->ProcResV20.iir_sigma = int(sigma);  //0~255, (8bit) sigma
    ProcRes->ProcResV20.iir_wt_sigma = int(wt_sigma * 8 + 0.5); //       (11bit),8bit+3bit, wt_sigma
    ProcRes->ProcResV20.iir_air_sigma = int(air_sigma);  //       (8bit) air_sigma
    ProcRes->ProcResV20.iir_tmax_sigma = int(tmax_sigma * 1024 + 0.5);  //       (11bit) tmax_sigma
    ProcRes->ProcResV20.cfg_wt = int(cfg_wt * 256); //0~256, (9bit) cfg_wt
    ProcRes->ProcResV20.cfg_air = int(cfg_air);  //0~255, (8bit) cfg_air
    ProcRes->ProcResV20.cfg_tmax = int(cfg_tmax * 1024); //0~1024,(11bit) cfg_tmax
    ProcRes->ProcResV20.dc_thed      = int(dc_thed);  //0~255, (8bit) dc_thed
    ProcRes->ProcResV20.dc_weitcur       = int(dc_weitcur * 256 + 0.5); //0~256, (9bit) dc_weitcur
    ProcRes->ProcResV20.air_thed     = int(air_thed);  //0~255, (8bit) air_thed
    ProcRes->ProcResV20.air_weitcur      = int(air_weitcur * 256 + 0.5);  //0~256, (9bit) air_weitcur
    ProcRes->ProcResV20.gaus_h0    = int(gaus_h[4]);//h0~h2  从大到小
    ProcRes->ProcResV20.gaus_h1    = int(gaus_h[1]);
    ProcRes->ProcResV20.gaus_h2    = int(gaus_h[0]);
    ProcRes->ProcResV20.sw_dhaz_dc_bf_h0   = int(dc_bf_h[12]);//h0~h5  从大到小
    ProcRes->ProcResV20.sw_dhaz_dc_bf_h1   = int(dc_bf_h[7]);
    ProcRes->ProcResV20.sw_dhaz_dc_bf_h2   = int(dc_bf_h[6]);
    ProcRes->ProcResV20.sw_dhaz_dc_bf_h3   = int(dc_bf_h[2]);
    ProcRes->ProcResV20.sw_dhaz_dc_bf_h4   = int(dc_bf_h[1]);
    ProcRes->ProcResV20.sw_dhaz_dc_bf_h5   = int(dc_bf_h[0]);
    ProcRes->ProcResV20.air_bf_h0  = int(air_bf_h[4]);//h0~h2  从大到小
    ProcRes->ProcResV20.air_bf_h1  = int(air_bf_h[1]);
    ProcRes->ProcResV20.air_bf_h2  = int(air_bf_h[0]);


    LOG1_ADEHAZE("EIXT: %s \n", __func__);
}

void GetDehazeParamsV21(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    float air_lc_en = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.air_lc_en, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_self_adp[7]
    float dc_min_th = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.dc_min_th, iso, RK_DEHAZE_ISO_NUM);
    float dc_max_th = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.dc_max_th, iso, RK_DEHAZE_ISO_NUM);
    float yhist_th = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.yhist_th, iso,  RK_DEHAZE_ISO_NUM);
    float yblk_th = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.yblk_th, iso, RK_DEHAZE_ISO_NUM);
    float dark_th = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.dark_th, iso, RK_DEHAZE_ISO_NUM);
    float bright_min = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.bright_min, iso, RK_DEHAZE_ISO_NUM);
    float bright_max = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.bright_max, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_range_adj[6]
    float wt_max = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.wt_max, iso, RK_DEHAZE_ISO_NUM);
    float air_max = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.air_max, iso, RK_DEHAZE_ISO_NUM);
    float air_min = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.air_min, iso, RK_DEHAZE_ISO_NUM);
    float tmax_base = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.tmax_base, iso, RK_DEHAZE_ISO_NUM);
    float tmax_off = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.tmax_off, iso, RK_DEHAZE_ISO_NUM);
    float tmax_max = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.tmax_max, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_iir_control[5]
    float stab_fnum = para->calib_v21[mode].dehaze_setting.IIR_setting.stab_fnum;
    float sigma = para->calib_v21[mode].dehaze_setting.IIR_setting.sigma;
    float wt_sigma = para->calib_v21[mode].dehaze_setting.IIR_setting.wt_sigma;
    float air_sigma = para->calib_v21[mode].dehaze_setting.IIR_setting.air_sigma;
    float tmax_sigma = para->calib_v21[mode].dehaze_setting.IIR_setting.tmax_sigma;
    float pre_wet = para->calib_v21[mode].dehaze_setting.IIR_setting.pre_wet;

    float cfg_wt = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.cfg_wt, iso, RK_DEHAZE_ISO_NUM);
    float cfg_air = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.cfg_air, iso, RK_DEHAZE_ISO_NUM);
    float cfg_tmax = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.cfg_tmax, iso, RK_DEHAZE_ISO_NUM);

    float range_sigma = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.range_sigma, iso, RK_DEHAZE_ISO_NUM);
    float space_sigma_cur = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.space_sigma_cur, iso, RK_DEHAZE_ISO_NUM);
    float space_sigma_pre = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.space_sigma_pre, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_bi_para[4]
    float bf_weight = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.bf_weight, iso, RK_DEHAZE_ISO_NUM);
    float dc_weitcur = LinearInterp(para->calib_v21[mode].dehaze_setting.iso, para->calib_v21[mode].dehaze_setting.dc_weitcur, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_air_bf_h[9],dehaze_gaus_h[9]
    float gaus_h[9] = {2.0000, 4.0000, 2.0000,
                       4.0000, 8.0000, 4.0000,
                       2.0000, 4.0000, 2.0000
                      };

    int rawWidth = 1920;
    int rawHeight = 1080;
    ProcRes->ProcResV21.air_lc_en    = int(air_lc_en); // air_lc_en
    ProcRes->ProcResV21.dc_min_th    = int(dc_min_th); //0~255, (8bit) dc_min_th
    ProcRes->ProcResV21.dc_max_th    = int(dc_max_th);  //0~255, (8bit) dc_max_th
    ProcRes->ProcResV21.yhist_th    = int(yhist_th);  //0~255, (8bit) yhist_th
    ProcRes->ProcResV21.yblk_th    = int(yblk_th * ((rawWidth + 15) / 16) * ((rawHeight + 15) / 16)); //default:28,(9bit) yblk_th
    ProcRes->ProcResV21.dark_th    = int(dark_th);  //0~255, (8bit) dark_th
    ProcRes->ProcResV21.bright_min   = int(bright_min);  //0~255, (8bit) bright_min
    ProcRes->ProcResV21.bright_max   = int(bright_max);  //0~255, (8bit) bright_max
    ProcRes->ProcResV21.wt_max   = int(wt_max * 256); //0~255, (8bit) wt_max
    ProcRes->ProcResV21.air_min   = int(air_min);  //0~255, (8bit) air_min
    ProcRes->ProcResV21.air_max   = int(air_max);  //0~256, (8bit) air_max
    ProcRes->ProcResV21.tmax_base   = int(tmax_base);  //0~255, (8bit) tmax_base
    ProcRes->ProcResV21.tmax_off   = int(tmax_off * 1024); //0~1024,(10bit) tmax_off
    ProcRes->ProcResV21.tmax_max   = int(tmax_max * 1024); //0~1024,(10bit) tmax_max
    ProcRes->ProcResV21.stab_fnum = int(stab_fnum);  //1~31,  (5bit) stab_fnum
    ProcRes->ProcResV21.iir_sigma = int(sigma);  //0~255, (8bit) sigma
    ProcRes->ProcResV21.iir_wt_sigma = int(wt_sigma * 8 + 0.5); //       (11bit),8bit+3bit, wt_sigma
    ProcRes->ProcResV21.iir_air_sigma = int(air_sigma);  //       (8bit) air_sigma
    ProcRes->ProcResV21.iir_tmax_sigma = int(tmax_sigma * 1024 + 0.5);  //       (11bit) tmax_sigma
    ProcRes->ProcResV21.iir_pre_wet = int(pre_wet * 128 + 0.5);  //       (7bit) iir_pre_wet
    ProcRes->ProcResV21.cfg_wt = int(cfg_wt * 256); //0~256, (9bit) cfg_wt
    ProcRes->ProcResV21.cfg_air = int(cfg_air);  //0~255, (8bit) cfg_air
    ProcRes->ProcResV21.cfg_tmax = int(cfg_tmax * 1024); //0~1024,(11bit) cfg_tmax
    ProcRes->ProcResV21.range_sima = int(range_sigma * 512); //0~512,(9bit) range_sima
    ProcRes->ProcResV21.space_sigma_cur = int(space_sigma_cur * 256); //0~256,(8bit) space_sigma_cur
    ProcRes->ProcResV21.space_sigma_pre = int(space_sigma_pre * 256); //0~256,(8bit) space_sigma_pre
    ProcRes->ProcResV21.bf_weight      = int(bf_weight * 512); //0~512, (9bit) dc_thed
    ProcRes->ProcResV21.dc_weitcur       = int(dc_weitcur * 256 + 0.5); //0~256, (9bit) dc_weitcur
    ProcRes->ProcResV21.gaus_h0    = int(gaus_h[4]);//h0~h2  从大到小
    ProcRes->ProcResV21.gaus_h1    = int(gaus_h[1]);
    ProcRes->ProcResV21.gaus_h2    = int(gaus_h[0]);


    LOG1_ADEHAZE("EIXT: %s \n", __func__);
}

void GetEnhanceParams(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    float enhance_value = LinearInterp(para->calib_v20[mode].enhance_setting.iso, para->calib_v20[mode].enhance_setting.enhance_value, iso, RK_DEHAZE_ISO_NUM);
    ProcRes->ProcResV21.enhance_value = int(enhance_value * 1024 + 0.5); //       (14bit),4bit + 10bit, enhance_value

    LOGD_ADEHAZE("%s enhance_en:%d enhance_value:%f enhance_value:%d\n", __func__,
                 ProcRes->ProcResV20.enhance_en, enhance_value, ProcRes->ProcResV21.enhance_value);

    LOG1_ADEHAZE("EIXT: %s \n", __func__);
}

void GetEnhanceParamsV21(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    float enhance_value = LinearInterp(para->calib_v21[mode].enhance_setting.iso, para->calib_v21[mode].enhance_setting.enhance_value, iso, RK_DEHAZE_ISO_NUM);
    float enhance_chroma = LinearInterp(para->calib_v21[mode].enhance_setting.iso, para->calib_v21[mode].enhance_setting.enhance_chroma, iso, RK_DEHAZE_ISO_NUM);

    ProcRes->ProcResV21.enhance_value = int(enhance_value * 1024 + 0.5); //       (14bit),4bit + 10bit, enhance_value
    ProcRes->ProcResV21.enhance_chroma = int(enhance_chroma * 1024 + 0.5); //       (14bit),4bit + 10bit, enhance_value

    for(int i = 0; i < 17; i++)
        ProcRes->ProcResV21.enh_curve[i] = (int)(para->calib_v21[mode].enhance_setting.enhance_curve[i]);

    LOGD_ADEHAZE("%s enhance_en:%d enhance_value:%f enhance_chroma:%f\n", __func__,
                 ProcRes->ProcResV21.enhance_en, enhance_value, enhance_chroma);
    LOGD_ADEHAZE("%s enhance_value_reg:%d enhance_chroma_reg:%d\n", __func__,
                 ProcRes->ProcResV21.enhance_value, ProcRes->ProcResV21.enhance_chroma);

    LOG1_ADEHAZE("EIXT: %s \n", __func__);
}

void GetHistParams(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    float hist_channel = LinearInterpEnable(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_channel, iso, RK_DEHAZE_ISO_NUM);
    float hist_para_en = LinearInterpEnable(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_para_en, iso, RK_DEHAZE_ISO_NUM);
    float hist_gratio = LinearInterp(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_gratio, iso, RK_DEHAZE_ISO_NUM);
    float hist_th_off = LinearInterp(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_th_off, iso, RK_DEHAZE_ISO_NUM);
    float hist_k = LinearInterp(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_k, iso, RK_DEHAZE_ISO_NUM);
    float hist_min = LinearInterp(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_min, iso, RK_DEHAZE_ISO_NUM);
    float hist_scale = LinearInterp(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.hist_scale, iso, RK_DEHAZE_ISO_NUM);
    float cfg_gratio = LinearInterp(para->calib_v20[mode].hist_setting.iso, para->calib_v20[mode].hist_setting.cfg_gratio, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_hist_t0[6],dehaze_hist_t1[6],dehaze_hist_t2[6]
    float hist_conv_t0[6] = {1.0000, 2.0000, 1.0000, -1.0000, -2.0000, -1.0000};
    float hist_conv_t1[6] = {1.0000, 0.0000, -1.0000, 2.0000, 0.0000, -2.0000};
    float hist_conv_t2[6] = {1.0000, -2.0000, 1.0000, 2.0000, -4.0000, 2.0000};

    ProcRes->ProcResV20.hist_chn = int(hist_channel); //  hist_para_en
    ProcRes->ProcResV20.hpara_en = int(hist_para_en); //  hist_para_en
    ProcRes->ProcResV20.hist_gratio   = int(hist_gratio * 8); //       (8bit) hist_gratio
    ProcRes->ProcResV20.hist_th_off   = int(hist_th_off);  //       (8bit) hist_th_off
    ProcRes->ProcResV20.hist_k   = int(hist_k * 4 + 0.5); //0~7    (5bit),3bit+2bit, hist_k
    ProcRes->ProcResV20.hist_min   = int(hist_min * 256); //       (9bit) hist_min
    ProcRes->ProcResV20.cfg_gratio = int(cfg_gratio * 256); //       (13bit),5bit+8bit, cfg_gratio
    ProcRes->ProcResV20.hist_scale       = int(hist_scale *  256 + 0.5 );  //       (13bit),5bit + 8bit, sw_hist_scale

    for (int i = 0; i < 6; i++)
    {
        ProcRes->ProcResV20.conv_t0[i]     = int(hist_conv_t0[i]);
        ProcRes->ProcResV20.conv_t1[i]     = int(hist_conv_t1[i]);
        ProcRes->ProcResV20.conv_t2[i]     = int(hist_conv_t2[i]);
    }
    LOGD_ADEHAZE("%s hist_channel:%d hist_para_en:%d hist_gratio:%f hist_th_off:%f hist_k:%f hist_min:%f hist_scale:%f cfg_gratio:%f\n", __func__,
                 ProcRes->ProcResV20.hist_chn, ProcRes->ProcResV20.hpara_en, hist_gratio, hist_th_off, hist_k, hist_min, hist_scale, cfg_gratio);
    LOGD_ADEHAZE("%s hist_gratio_reg:%d hist_th_off_reg:%d hist_k_reg:%d hist_min_reg:%d hist_scale_reg:%d cfg_gratio_reg:%d\n", __func__,
                 ProcRes->ProcResV20.hist_gratio, ProcRes->ProcResV20.hist_th_off, ProcRes->ProcResV20.hist_k,
                 ProcRes->ProcResV20.hist_min, ProcRes->ProcResV20.hist_scale, ProcRes->ProcResV20.cfg_gratio);

    LOG1_ADEHAZE("EIXT: %s \n", __func__);

}

void GetHistParamsV21(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);


    float hist_para_en = LinearInterpEnable(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.hist_para_en, iso, RK_DEHAZE_ISO_NUM);
    float hist_gratio = LinearInterp(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.hist_gratio, iso, RK_DEHAZE_ISO_NUM);
    float hist_th_off = LinearInterp(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.hist_th_off, iso, RK_DEHAZE_ISO_NUM);
    float hist_k = LinearInterp(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.hist_k, iso, RK_DEHAZE_ISO_NUM);
    float hist_min = LinearInterp(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.hist_min, iso, RK_DEHAZE_ISO_NUM);
    float hist_scale = LinearInterp(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.hist_scale, iso, RK_DEHAZE_ISO_NUM);
    float cfg_gratio = LinearInterp(para->calib_v21[mode].hist_setting.iso, para->calib_v21[mode].hist_setting.cfg_gratio, iso, RK_DEHAZE_ISO_NUM);

    // dehaze_hist_t0[6],dehaze_hist_t1[6],dehaze_hist_t2[6]
    float hist_conv_t0[6] = {1.0000, 2.0000, 1.0000, -1.0000, -2.0000, -1.0000};
    float hist_conv_t1[6] = {1.0000, 0.0000, -1.0000, 2.0000, 0.0000, -2.0000};
    float hist_conv_t2[6] = {1.0000, -2.0000, 1.0000, 2.0000, -4.0000, 2.0000};

    ProcRes->ProcResV21.hpara_en = int(hist_para_en); //  hist_para_en
    ProcRes->ProcResV21.hist_gratio   = int(hist_gratio * 8); //       (8bit) hist_gratio
    ProcRes->ProcResV21.hist_th_off   = int(hist_th_off);  //       (8bit) hist_th_off
    ProcRes->ProcResV21.hist_k   = int(hist_k * 4 + 0.5); //0~7    (5bit),3bit+2bit, hist_k
    ProcRes->ProcResV21.hist_min   = int(hist_min * 256); //       (9bit) hist_min
    ProcRes->ProcResV21.cfg_gratio = int(cfg_gratio * 256); //       (13bit),5bit+8bit, cfg_gratio
    ProcRes->ProcResV21.hist_scale       = int(hist_scale *  256 + 0.5 );  //       (13bit),5bit + 8bit, sw_hist_scale

    LOGD_ADEHAZE("%s hist_para_en:%d hist_gratio:%f hist_th_off:%f hist_k:%f hist_min:%f hist_scale:%f cfg_gratio:%f\n", __func__,
                 ProcRes->ProcResV21.hpara_en, hist_gratio, hist_th_off, hist_k, hist_min, hist_scale, cfg_gratio);
    LOGD_ADEHAZE("%s hist_gratio_reg:%d hist_th_off_reg:%d hist_k_reg:%d hist_min_reg:%d hist_scale_reg:%d cfg_gratio_reg:%d\n", __func__,
                 ProcRes->ProcResV21.hist_gratio, ProcRes->ProcResV21.hist_th_off, ProcRes->ProcResV21.hist_k,
                 ProcRes->ProcResV21.hist_min, ProcRes->ProcResV21.hist_scale, ProcRes->ProcResV21.cfg_gratio);

    LOG1_ADEHAZE("EIXT: %s \n", __func__);

}

void AdehazeApiToolProcess(AdehazeHandle_t* para, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    //cfg setting
    para->ProcRes.ProcResV20.cfg_alpha = (int)LIMIT_VALUE((para->AdehazeAtrr.stTool.calib_v20[mode].cfg_alpha * 256.0), 255, 0);
    LOGD_ADEHAZE("%s Config Alpha:%d\n", __func__, para->ProcRes.ProcResV20.cfg_alpha);

    //fuction enable
    EnableSetting(&para->AdehazeAtrr.stTool, &para->ProcRes, iso, mode);

    //dehaze setting
    GetDehazeParams(&para->AdehazeAtrr.stTool, &para->ProcRes, iso, mode);

    //enhance setting
    GetEnhanceParams(&para->AdehazeAtrr.stTool, &para->ProcRes, iso, mode);

    //hist setting
    GetHistParams(&para->AdehazeAtrr.stTool, &para->ProcRes, iso, mode);

    LOG1_ADEHAZE("EXIT: %s \n", __func__);

}

void AdehazeEnhanceApiOnProcess(AdehazeHandle_t* para, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);
    LOGD_ADEHAZE(" %s: Adehaze Api on!!!, api mode:%d \n", __func__, para->AdehazeAtrr.mode);

    if(para->AdehazeAtrr.mode == RK_AIQ_DEHAZE_MODE_OFF)
    {
        //cfg setting
        para->ProcRes.ProcResV20.cfg_alpha = (int)LIMIT_VALUE((para->calib_dehaz.calib_v20[mode].cfg_alpha * 256.0), 255, 0);
        LOGD_ADEHAZE("%s Config Alpha:%d\n", __func__, para->ProcRes.ProcResV20.cfg_alpha);

        //enable setting
        EnableSetting(&para->calib_dehaz, &para->ProcRes, iso, mode);

        //dehaze seting
        GetDehazeParams(&para->calib_dehaz, &para->ProcRes, iso, mode);

        //enhance setting
        GetEnhanceParams(&para->calib_dehaz, &para->ProcRes, iso, mode);

        //hist setting
        GetHistParams(&para->calib_dehaz, &para->ProcRes, iso, mode);

    }
    else if(para->AdehazeAtrr.mode > RK_AIQ_DEHAZE_MODE_INVALID && para->AdehazeAtrr.mode < RK_AIQ_DEHAZE_MODE_OFF)
    {
        para->ProcRes.ProcResV20.dc_en = 0x1;
        para->ProcRes.ProcResV20.enhance_en = 0x1;
        LOGD_ADEHAZE(" Dehaze fuction en:%d, Dehaze en:%d, Enhance en:%d,", 0x1, 0, 0x1 );

        if(para->AdehazeAtrr.mode == RK_AIQ_DEHAZE_MODE_AUTO )
            para->ProcRes.ProcResV20.cfg_alpha = (int)LIMIT_VALUE((para->calib_dehaz.calib_v20[mode].cfg_alpha * 256.0), 255, 0);
        else if(para->AdehazeAtrr.mode == RK_AIQ_DEHAZE_MODE_MANUAL)
            para->ProcRes.ProcResV20.cfg_alpha = 255;
        LOGD_ADEHAZE("%s Config Alpha:%d\n", __func__, para->ProcRes.ProcResV20.cfg_alpha);

        //hist en setting
        if(para->calib_dehaz.calib_v20[mode].hist_setting.en != 0)
            para->ProcRes.ProcResV20.hist_en = 0x1;
        else
            para->ProcRes.ProcResV20.hist_en = 0;

        LOGD_ADEHAZE(" Hist en:%d\n", para->ProcRes.ProcResV20.hist_en );

        GetDehazeParams(&para->calib_dehaz, &para->ProcRes, iso, mode);

        if(para->AdehazeAtrr.mode == RK_AIQ_DEHAZE_MODE_MANUAL)
        {
            float level = (float)(para->AdehazeAtrr.stManual.strength);
            float level_default = 5;
            float level_diff = (float)(level - level_default);

            //sw_dhaz_cfg_wt
            float sw_dhaz_cfg_wt = (float)para->ProcRes.ProcResV20.cfg_wt;
            sw_dhaz_cfg_wt += level_diff * 0.05;
            sw_dhaz_cfg_wt = LIMIT_VALUE(sw_dhaz_cfg_wt, 0.99, 0.01);
            para->ProcRes.ProcResV20.cfg_wt = (int)sw_dhaz_cfg_wt;

            //sw_dhaz_cfg_air
            float sw_dhaz_cfg_air = (float)para->ProcRes.ProcResV20.cfg_air;
            sw_dhaz_cfg_air += level_diff * 5;
            sw_dhaz_cfg_air = LIMIT_VALUE(sw_dhaz_cfg_air, 255, 0.01);
            para->ProcRes.ProcResV20.cfg_air = (int)sw_dhaz_cfg_air;

            //sw_dhaz_cfg_tmax
            float sw_dhaz_cfg_tmax = (float)para->ProcRes.ProcResV20.cfg_tmax;
            sw_dhaz_cfg_tmax += level_diff * 0.05;
            sw_dhaz_cfg_tmax = LIMIT_VALUE(sw_dhaz_cfg_tmax, 0.99, 0.01);
            para->ProcRes.ProcResV20.cfg_tmax = (int)sw_dhaz_cfg_tmax;

            LOGD_ADEHAZE(" %s: Adehaze munual level:%f level_diff:%f\n", __func__, level, level_diff);
            LOGD_ADEHAZE(" %s: After manual api sw_dhaz_cfg_wt:%f sw_dhaz_cfg_air:%f sw_dhaz_cfg_tmax:%f\n", __func__, sw_dhaz_cfg_wt,
                         sw_dhaz_cfg_air, sw_dhaz_cfg_tmax);
        }
        //hist setting
        GetHistParams(&para->calib_dehaz, &para->ProcRes, iso, mode);

    }
    else if(para->AdehazeAtrr.mode == RK_AIQ_DEHAZE_MODE_TOOL)
        AdehazeApiToolProcess(para, iso, mode);
    else
        LOGE_ADEHAZE("%s:Wrong Adehaze API mode!!! \n", __func__);


    LOG1_ADEHAZE("EXIT: %s \n", __func__);

}

void AdehazeEnhanceApiOnProcessV21(AdehazeHandle_t* para, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);
    LOGD_ADEHAZE(" %s: Adehaze Api on!!!, api mode:%d \n", __func__, para->AdehazeAtrr.mode);




    LOG1_ADEHAZE("EXIT: %s \n", __func__);

}

void AdehazeEnhanceApiOffProcess(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);
    LOGD_ADEHAZE(" %s: Adehaze Api off!!!\n", __func__);

    //cfg setting
    ProcRes->ProcResV20.cfg_alpha = (int)LIMIT_VALUE((para->calib_v20[mode].cfg_alpha * 256.0), 255, 0);
    LOGD_ADEHAZE("%s Config Alpha:%d\n", __func__, ProcRes->ProcResV20.cfg_alpha);

    //enable setting
    EnableSetting(para, ProcRes, iso, mode);

    //dehaze setting
    GetDehazeParams(para, ProcRes, iso, mode);

    //enhance setting
    GetEnhanceParams(para, ProcRes, iso, mode);

    //hist setting
    GetHistParams(para, ProcRes, iso, mode);

    LOG1_ADEHAZE("EXIT: %s \n", __func__);

}

void AdehazeEnhanceApiOffProcessV21(CalibDb_Dehaze_t* para, RkAiqAdehazeProcResult_t* ProcRes, int iso, int mode)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);
    LOGD_ADEHAZE(" %s: Adehaze Api off!!!\n", __func__);

    //enable setting
    EnableSettingV21(para, ProcRes, iso, mode);

    //cfg setting
    ProcRes->ProcResV21.cfg_alpha = (int)LIMIT_VALUE((para->calib_v21[mode].cfg_alpha * 256.0), 255, 0);

    //dehaze setting
    GetDehazeParamsV21(para, ProcRes, iso, mode);

    //enhance setting
    GetEnhanceParamsV21(para, ProcRes, iso, mode);

    //hist setting
    GetHistParamsV21(para, ProcRes, iso, mode);

    LOG1_ADEHAZE("EXIT: %s \n", __func__);

}

XCamReturn AdehazeProcessV21(AdehazeHandle_t* para, int iso, int mode, int version)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    para->AdehazeAtrr.byPass = true;
    if(!(para->AdehazeAtrr.byPass))
        AdehazeEnhanceApiOnProcessV21(para, iso, mode);
    else {
        if(para->calib_dehaz.calib_v21[mode].en == 0)
            memset(&para->ProcRes.ProcResV21, 0, sizeof(AdehazeV21ProcResult_t));
        else
            AdehazeEnhanceApiOffProcessV21(&para->calib_dehaz,
                                           &para->ProcRes, iso, mode);
    }

    LOG1_ADEHAZE("EXIT: %s \n", __func__);
    return ret;
}
void AdehazeGetStats
(
    AdehazeHandle_t*           pAdehazeCtx,
    rkisp_adehaze_stats_t*         ROData
) {
    LOG1_AHDR( "%s:enter!\n", __FUNCTION__);
    LOGV_ADEHAZE("%s:  Ahdr RO data from register:\n", __FUNCTION__);

    if(pAdehazeCtx->HWversion == 0) {
        pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_air_base = ROData->dehaze_stats_v20.dhaz_adp_air_base;
        pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_wt = ROData->dehaze_stats_v20.dhaz_adp_wt;
        pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_gratio = ROData->dehaze_stats_v20.dhaz_adp_gratio;
        pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_tmax = ROData->dehaze_stats_v20.dhaz_adp_tmax;
        for(int i = 0; i < 64; i++) {
            pAdehazeCtx->stats.dehaze_stats_v20.h_b_iir[i] = ROData->dehaze_stats_v20.h_b_iir[i];
            pAdehazeCtx->stats.dehaze_stats_v20.h_g_iir[i] = ROData->dehaze_stats_v20.h_g_iir[i];
            pAdehazeCtx->stats.dehaze_stats_v20.h_r_iir[i] = ROData->dehaze_stats_v20.h_r_iir[i];
        }

        LOGV_ADEHAZE("%s:  dhaz_adp_air_base:%d dhaz_adp_wt:%d dhaz_adp_gratio:%d dhaz_adp_tmax:%d\n", __FUNCTION__,
                     pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_air_base, pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_wt,
                     pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_gratio, pAdehazeCtx->stats.dehaze_stats_v20.dhaz_adp_tmax);
        for(int i = 0; i < 64; i++) {
            LOGV_ADEHAZE("%s:  h_b_iir[%d]:%d:\n", __FUNCTION__, i, pAdehazeCtx->stats.dehaze_stats_v20.h_b_iir[i]);
            LOGV_ADEHAZE("%s:  h_g_iir[%d]:%d:\n", __FUNCTION__, i, pAdehazeCtx->stats.dehaze_stats_v20.h_g_iir[i]);
            LOGV_ADEHAZE("%s:  h_r_iir[%d]:%d:\n", __FUNCTION__, i, pAdehazeCtx->stats.dehaze_stats_v20.h_r_iir[i]);
        }
    }
    else if(pAdehazeCtx->HWversion == 1) {
        pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_air_base = ROData->dehaze_stats_v21.dhaz_adp_air_base;
        pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_wt = ROData->dehaze_stats_v21.dhaz_adp_wt;
        pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_gratio = ROData->dehaze_stats_v21.dhaz_adp_gratio;
        pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_tmax = ROData->dehaze_stats_v21.dhaz_adp_tmax;
        for(int i = 0; i < 64; i++)
            pAdehazeCtx->stats.dehaze_stats_v21.h_rgb_iir[i] = ROData->dehaze_stats_v21.h_rgb_iir[i];

        LOGV_ADEHAZE("%s:  dhaz_adp_air_base:%d dhaz_adp_wt:%d dhaz_adp_gratio:%d dhaz_adp_tmax:%d\n", __FUNCTION__,
                     pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_air_base, pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_wt,
                     pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_gratio, pAdehazeCtx->stats.dehaze_stats_v21.dhaz_adp_tmax);
        for(int i = 0; i < 64; i++)
            LOGV_ADEHAZE("%s:  h_rgb_iir[%d]:%d:\n", __FUNCTION__, i, pAdehazeCtx->stats.dehaze_stats_v21.h_rgb_iir[i]);
    }

    //get other stats from stats
    for(int i = 0; i < 225; i++)
    {
        pAdehazeCtx->stats.other_stats.short_luma[i] = ROData->other_stats.short_luma[i];
        pAdehazeCtx->stats.other_stats.long_luma[i] = ROData->other_stats.long_luma[i];
        pAdehazeCtx->stats.other_stats.tmo_luma[i] = ROData->other_stats.tmo_luma[i];
    }

    if(pAdehazeCtx->FrameNumber == 3)
    {
        for(int i = 0; i < 25; i++)
            pAdehazeCtx->stats.other_stats.middle_luma[i] = ROData->other_stats.middle_luma[i];
    }

    LOG1_AHDR( "%s:exit!\n", __FUNCTION__);
}

XCamReturn AdehazeInit(AdehazeHandle_t** para, CamCalibDbContext_t* calib)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    AdehazeHandle_t *handle = (AdehazeHandle_t*)malloc(sizeof(AdehazeHandle_t));
    if (NULL == handle)
        return XCAM_RETURN_ERROR_MEM;
    memset(handle, 0, sizeof(AdehazeHandle_t));
    handle->pCalibDb = calib;
    handle->calib_dehaz = calib->dehaze;
    handle->AdehazeAtrr.stTool = calib->dehaze;
    handle->AdehazeAtrr.byPass = true;
    handle->AdehazeAtrr.mode = RK_AIQ_DEHAZE_MODE_AUTO;
    *para = handle;
    LOG1_ADEHAZE("EXIT: %s \n", __func__);
    return(ret);
}

XCamReturn AdehazeRelease(AdehazeHandle_t* para)
{
    LOG1_ADEHAZE("ENTER: %s \n", __func__);
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (para)
        free(para);
    LOG1_ADEHAZE("EXIT: %s \n", __func__);
    return(ret);
}

XCamReturn AdehazeProcess(AdehazeHandle_t* para, int iso, int mode, int version)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    LOG1_ADEHAZE("ENTER: %s \n", __func__);

    if(version == 0) {
        //big mode
        para->ProcRes.ProcResV20.big_en = para->width > DEHAZEBIGMODE ? 1 : 0;
        para->ProcRes.ProcResV20.nobig_en = (int)(1 - para->ProcRes.ProcResV20.big_en);

        LOGD_ADEHAZE("%s ISO:%d mode:%d\n", __func__, iso, mode);

        if(!(para->AdehazeAtrr.byPass))
            AdehazeEnhanceApiOnProcess(para, iso, mode);
        else
            AdehazeEnhanceApiOffProcess(&para->calib_dehaz,
                                        &para->ProcRes, iso, mode);
    }
    else if(version == 1)
        AdehazeProcessV21(para, iso, mode, version);
    else
        LOGE_ADEHAZE(" %s:Wrong hardware version!! \n", __func__);

    LOG1_ADEHAZE("EXIT: %s \n", __func__);
    return ret;
}

