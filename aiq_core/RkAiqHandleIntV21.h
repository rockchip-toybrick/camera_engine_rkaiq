/*
 * RkAiqHandleIntV1.h
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

#ifndef _RK_AIQ_HANDLE_INT_V1_H_
#define _RK_AIQ_HANDLE_INT_V1_H_

#include "RkAiqHandleInt.h"

namespace RkCam {

class RkAiqAdrcV1HandleInt:
    virtual public RkAiqAdrcHandle,
    virtual public RkAiqHandleIntCom {
public:
    explicit RkAiqAdrcV1HandleInt(RkAiqAlgoDesComm* des, RkAiqCore* aiqCore)
        : RkAiqHandle(des, aiqCore)
        , RkAiqAdrcHandle(des, aiqCore)
        , RkAiqHandleIntCom(des, aiqCore) {};
    virtual ~RkAiqAdrcV1HandleInt() {
        RkAiqAdrcHandle::deInit();
    };
    virtual XCamReturn updateConfig(bool needSync);
    virtual XCamReturn prepare();
    virtual XCamReturn preProcess();
    virtual XCamReturn processing();
    virtual XCamReturn postProcess();
    //XCamReturn setAttrib(adrcV1_attrib_t att);
    //XCamReturn getAttrib(adrcV1_attrib_t *att);
protected:
    virtual void init();
    virtual void deInit() {
        RkAiqAdrcHandle::deInit();
    };
private:
    //adrcV1_attrib_t mCurAtt;
    //adrcV1_attrib_t mNewAtt;
};





// asharp v3
class RkAiqAsharpV3HandleInt:
    virtual public RkAiqAsharpHandle,
    virtual public RkAiqHandleIntCom {
public:
    explicit RkAiqAsharpV3HandleInt(RkAiqAlgoDesComm* des, RkAiqCore* aiqCore)
        : RkAiqHandle(des, aiqCore)
        , RkAiqAsharpHandle(des, aiqCore)
        , RkAiqHandleIntCom(des, aiqCore) {
        memset(&mCurAtt, 0, sizeof(rk_aiq_sharp_attrib_v3_t));
        memset(&mNewAtt, 0, sizeof(rk_aiq_sharp_attrib_v3_t));
    };
    virtual ~RkAiqAsharpV3HandleInt() {
        RkAiqAsharpHandle::deInit();
    };
    virtual XCamReturn updateConfig(bool needSync);
    virtual XCamReturn prepare();
    virtual XCamReturn preProcess();
    virtual XCamReturn processing();
    virtual XCamReturn postProcess();
    // TODO add algo specific methords, this is a sample
    XCamReturn setAttrib(rk_aiq_sharp_attrib_v3_t *att);
    XCamReturn getAttrib(rk_aiq_sharp_attrib_v3_t *att);
    XCamReturn setStrength(float fPercent);
    XCamReturn getStrength(float *pPercent);
    XCamReturn setIQPara(rk_aiq_sharp_IQPara_V3_t *para);
    XCamReturn getIQPara(rk_aiq_sharp_IQPara_V3_t *para);

protected:
    virtual void init();
    virtual void deInit() {
        RkAiqAsharpHandle::deInit();
    };
private:
    // TODO
    rk_aiq_sharp_attrib_v3_t mCurAtt;
    rk_aiq_sharp_attrib_v3_t mNewAtt;
    rk_aiq_sharp_IQPara_V3_t mCurIQPara;
    rk_aiq_sharp_IQPara_V3_t mNewIQPara;
    bool updateIQpara = false;

};




// aynr v2
class RkAiqAynrV2HandleInt:
    virtual public RkAiqAynrHandle,
    virtual public RkAiqHandleIntCom {
public:
    explicit RkAiqAynrV2HandleInt(RkAiqAlgoDesComm* des, RkAiqCore* aiqCore)
        : RkAiqHandle(des, aiqCore)
        , RkAiqAynrHandle(des, aiqCore)
        , RkAiqHandleIntCom(des, aiqCore) {
        memset(&mCurAtt, 0, sizeof(rk_aiq_ynr_attrib_v2_t));
        memset(&mNewAtt, 0, sizeof(rk_aiq_ynr_attrib_v2_t));
    };
    virtual ~RkAiqAynrV2HandleInt() {
        RkAiqAynrHandle::deInit();
    };
    virtual XCamReturn updateConfig(bool needSync);
    virtual XCamReturn prepare();
    virtual XCamReturn preProcess();
    virtual XCamReturn processing();
    virtual XCamReturn postProcess();
    // TODO add algo specific methords, this is a sample
    XCamReturn setAttrib(rk_aiq_ynr_attrib_v2_t *att);
    XCamReturn getAttrib(rk_aiq_ynr_attrib_v2_t *att);
    XCamReturn setStrength(float fPercent);
    XCamReturn getStrength(float *pPercent);
    XCamReturn setIQPara(rk_aiq_ynr_IQPara_V2_t *pPara);
    XCamReturn getIQPara(rk_aiq_ynr_IQPara_V2_t *pPara);
protected:
    virtual void init();
    virtual void deInit() {
        RkAiqAynrHandle::deInit();
    };
private:
    // TODO
    rk_aiq_ynr_attrib_v2_t mCurAtt;
    rk_aiq_ynr_attrib_v2_t mNewAtt;
    rk_aiq_ynr_IQPara_V2_t mCurIQPara;
    rk_aiq_ynr_IQPara_V2_t mNewIQPara;
    bool updateIQpara = false;
};





// acnr v1
class RkAiqAcnrV1HandleInt:
    virtual public RkAiqAcnrHandle,
    virtual public RkAiqHandleIntCom {
public:
    explicit RkAiqAcnrV1HandleInt(RkAiqAlgoDesComm* des, RkAiqCore* aiqCore)
        : RkAiqHandle(des, aiqCore)
        , RkAiqAcnrHandle(des, aiqCore)
        , RkAiqHandleIntCom(des, aiqCore) {
        memset(&mCurAtt, 0, sizeof(rk_aiq_cnr_attrib_v1_t));
        memset(&mNewAtt, 0, sizeof(rk_aiq_cnr_attrib_v1_t));
    };
    virtual ~RkAiqAcnrV1HandleInt() {
        RkAiqAcnrHandle::deInit();
    };
    virtual XCamReturn updateConfig(bool needSync);
    virtual XCamReturn prepare();
    virtual XCamReturn preProcess();
    virtual XCamReturn processing();
    virtual XCamReturn postProcess();
    // TODO add algo specific methords, this is a sample
    XCamReturn setAttrib(rk_aiq_cnr_attrib_v1_t *att);
    XCamReturn getAttrib(rk_aiq_cnr_attrib_v1_t *att);
    XCamReturn setStrength(float fPercent);
    XCamReturn getStrength(float *pPercent);
    XCamReturn setIQPara(rk_aiq_cnr_IQPara_V1_t *pPara);
    XCamReturn getIQPara(rk_aiq_cnr_IQPara_V1_t *pPara);
protected:
    virtual void init();
    virtual void deInit() {
        RkAiqAcnrHandle::deInit();
    };
private:
    // TODO
    rk_aiq_cnr_attrib_v1_t mCurAtt;
    rk_aiq_cnr_attrib_v1_t mNewAtt;
    rk_aiq_cnr_IQPara_V1_t mCurIQPara;
    rk_aiq_cnr_IQPara_V1_t mNewIQPara;
    bool updateIQpara = false;
};






// aynr v2
class RkAiqArawnrV2HandleInt:
    virtual public RkAiqArawnrHandle,
    virtual public RkAiqHandleIntCom {
public:
    explicit RkAiqArawnrV2HandleInt(RkAiqAlgoDesComm* des, RkAiqCore* aiqCore)
        : RkAiqHandle(des, aiqCore)
        , RkAiqArawnrHandle(des, aiqCore)
        , RkAiqHandleIntCom(des, aiqCore) {
        memset(&mCurAtt, 0, sizeof(rk_aiq_bayernr_attrib_v2_t));
        memset(&mNewAtt, 0, sizeof(rk_aiq_bayernr_attrib_v2_t));
    };
    virtual ~RkAiqArawnrV2HandleInt() {
        RkAiqArawnrHandle::deInit();
    };
    virtual XCamReturn updateConfig(bool needSync);
    virtual XCamReturn prepare();
    virtual XCamReturn preProcess();
    virtual XCamReturn processing();
    virtual XCamReturn postProcess();
    // TODO add algo specific methords, this is a sample
    XCamReturn setAttrib(rk_aiq_bayernr_attrib_v2_t *att);
    XCamReturn getAttrib(rk_aiq_bayernr_attrib_v2_t *att);
    XCamReturn setStrength(float fPercent);
    XCamReturn getStrength(float *pPercent);
    XCamReturn setIQPara(rk_aiq_bayernr_IQPara_V2_t *pPara);
    XCamReturn getIQPara(rk_aiq_bayernr_IQPara_V2_t *pPara);
protected:
    virtual void init();
    virtual void deInit() {
        RkAiqArawnrHandle::deInit();
    };
private:
    // TODO
    rk_aiq_bayernr_attrib_v2_t mCurAtt;
    rk_aiq_bayernr_attrib_v2_t mNewAtt;
    rk_aiq_bayernr_IQPara_V2_t mCurIQPara;
    rk_aiq_bayernr_IQPara_V2_t mNewIQPara;
    bool updateIQpara = false;
};



}
#endif
