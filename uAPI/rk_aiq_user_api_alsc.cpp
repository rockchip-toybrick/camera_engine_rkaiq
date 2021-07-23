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

#include "rk_aiq_user_api_alsc.h"
#include "RkAiqHandleInt.h"

RKAIQ_BEGIN_DECLARE

#ifdef RK_SIMULATOR_HW
#define CHECK_USER_API_ENABLE
#endif

XCamReturn
rk_aiq_user_api_alsc_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_lsc_attrib_t attr)
{
    CHECK_USER_API_ENABLE(RK_AIQ_ALGO_TYPE_ALSC);
    /*  RkAiqAlscHandleInt* algo_handle =
          algoHandle<RkAiqAlscHandleInt>(sys_ctx, RK_AIQ_ALGO_TYPE_ALSC);

      if (algo_handle) {
          return algo_handle->setAttrib(attr);
      }*/

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_user_api_alsc_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_lsc_attrib_t *attr)
{
    RkAiqAlscHandleInt* algo_handle =
        algoHandle<RkAiqAlscHandleInt>(sys_ctx, RK_AIQ_ALGO_TYPE_ALSC);

    if (algo_handle) {
        return algo_handle->getAttrib(attr);
    }

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_user_api_alsc_QueryLscInfo(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_lsc_querry_info_t *lsc_querry_info)
{
    RkAiqAlscHandleInt* algo_handle =
        algoHandle<RkAiqAlscHandleInt>(sys_ctx, RK_AIQ_ALGO_TYPE_ALSC);

    if (algo_handle) {
        return algo_handle->queryLscInfo(lsc_querry_info);
    }

    return XCAM_RETURN_NO_ERROR;
}

RKAIQ_END_DECLARE
