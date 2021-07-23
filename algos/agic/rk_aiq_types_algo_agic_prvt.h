
#ifndef __RKAIQ_TYPES_ALGO_AGIC_PRVT_H__
#define __RKAIQ_TYPES_ALGO_AGIC_PRVT_H__

#include "base/xcam_common.h"
#include "rk_aiq_types_algo_agic_int.h"
#include "RkAiqCalibDbTypes.h"
#include "xcam_log.h"

#define RKAIQ_GIC_BITS (12)

#define GIC_SWAP(_T_,A,B) { _T_ tmp = (A); (A) = (B); (B) = tmp; }


typedef enum AgicState_e {
    AGIC_STATE_INVALID           = 0,
    AGIC_STATE_INITIALIZED       = 1,
    AGIC_STATE_STOPPED           = 2,
    AGIC_STATE_RUNNING           = 3,
    AGIC_STATE_LOCKED            = 4,
    AGIC_STATE_MAX
} AgicState_t;

typedef CalibDb_Gic_t AgicFullParam_t;

typedef struct AgicContext_s {
    AgicConfig_t ConfigData;
    AgicProcResult_t ProcRes;
    AgicState_t state;
    CamCalibDbContext_t* pCalibDb;
    AgicFullParam_t full_param;
    int HWversion; //0:isp2.0 1:isp2.1
    int Gic_Scene_mode;
    int working_mode;
} AgicContext_t;

typedef struct _RkAiqAlgoContext {
    AgicContext_t agicCtx;
} RkAiqAlgoContext;

XCamReturn AgicInit(AgicContext_t *ppAgicCtx);

XCamReturn AgicRelease(AgicContext_t *pAgicCtx);

XCamReturn AgicStart(AgicContext_t *pAgicCtx);

XCamReturn AgicStop(AgicContext_t *pAgicCtx);

XCamReturn AgicConfig(AgicContext_t *pAgicCtx, AgicConfig_t* pAgicConfig);

XCamReturn AgicReConfig(AgicContext_t *pAgicCtx, AgicConfig_t* pAgicConfig);

XCamReturn AgicPreProcess(AgicContext_t *pAgicCtx);

XCamReturn AgicProcess(AgicContext_t *pAgicCtx, int ISO, int mode, int HWversion);

XCamReturn AgicGetProcResult(AgicContext_t *pAgicCtx, int HWversion);

#endif//__RKAIQ_TYPES_ALGO_AGIC_PRVT_H__
