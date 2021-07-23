
#ifndef _RKAIQ_BAYERNR_V2_H_
#define _RKAIQ_BAYERNR_V2_H_

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "base/xcam_log.h"
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "RkAiqCalibApi.h"
#include "aynr2/rk_aiq_types_aynr_algo_prvt_v2.h"

XCAM_BEGIN_DECLARE

Aynr_result_t ynr_get_mode_by_name_V2(struct list_head* pCalibdbList, char *name, Calibdb_Ynr_V2_t** ppProfile);

Aynr_result_t ynr_get_setting_by_name_V2(struct list_head *pSettingList, char *name, Calibdb_Ynr_params_V2_t** ppSetting);

Aynr_result_t ynr_config_setting_param_V2(RK_YNR_Params_V2_t *pParams, struct list_head *pCalibdbList, char* param_mode, char * snr_name);

Aynr_result_t ynr_init_params_V2(RK_YNR_Params_V2_t *pYnrParams, Calibdb_Ynr_params_V2_t* pCalibParms);

Aynr_result_t ynr_select_params_by_ISO_V2(RK_YNR_Params_V2_t *pParams, RK_YNR_Params_V2_Select_t *pSelect, Aynr_ExpInfo_t *pExpInfo);

Aynr_result_t ynr_fix_transfer_V2(RK_YNR_Params_V2_Select_t* pSelect, RK_YNR_Fix_V2_t *pFix, float fStrength, Aynr_ExpInfo_t *pExpInfo);

Aynr_result_t ynr_fix_printf_V2(RK_YNR_Fix_V2_t * pFix);


XCAM_END_DECLARE


#endif  // BAYERNR_READ_PARM_H_

