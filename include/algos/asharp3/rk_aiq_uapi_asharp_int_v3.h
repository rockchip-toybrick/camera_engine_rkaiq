#ifndef _RK_AIQ_UAPI_AYNR_INT_V3_H_
#define _RK_AIQ_UAPI_AYNR_INT_V3_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "asharp3/rk_aiq_types_asharp_algo_int_v3.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.




#endif
