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

#include "CamHwInfo.h"
#include "mediactl/mediactl-priv.h"
#include <linux/v4l2-subdev.h>
#include <sys/mman.h>

using namespace XCam;

#define CAMHW_INFO (0x1)

// isp2x media info
#define MI_STR_ISP2X_MODEL_ISP0             "rkisp0"
#define MI_STR_ISP2X_MODEL_ISP1             "rkisp1"
#define MI_STR_ISP2X_MODEL_ISP              "rkisp" // equal to rkisp0
#define MI_STR_ISP2X_ISP_ISP_SUBDEV         "rkisp-isp-subdev"
#define MI_STR_ISP2X_ISP_CSI_SUBDEV         "rkisp-csi-subdev"
#define MI_STR_ISP2X_ISP_MPFBC_SUBDEV       "rkisp-mpfbc-subdev"
#define MI_STR_ISP2X_ISP_MAINPATH           "rkisp_mainpath"
#define MI_STR_ISP2X_ISP_SELFPATH           "rkisp_selfpath"
#define MI_STR_ISP2X_ISP_RAWWR0             "rkisp_rawwr0"
#define MI_STR_ISP2X_ISP_RAWWR1             "rkisp_rawwr1"
#define MI_STR_ISP2X_ISP_RAWWR2             "rkisp_rawwr2"
#define MI_STR_ISP2X_ISP_RAWWR3             "rkisp_rawwr3"
#define MI_STR_ISP2X_ISP_DMAPATH            "rkisp_dmapath"
#define MI_STR_ISP2X_ISP_RAWRD0_M           "rkisp_rawrd0_m"
#define MI_STR_ISP2X_ISP_RAWRD1_L           "rkisp_rawrd1_l"
#define MI_STR_ISP2X_ISP_RAWRD2_S           "rkisp_rawrd2_s"
#define MI_STR_ISP2X_ISP_STATS              "rkisp-statistics"
#define MI_STR_ISP2X_ISP_INPUT_PARAMS       "rkisp-input-params"
#define MI_STR_ISP2X_ISP_MIPI_LUMA          "rkisp-mipi-luma"
#define MI_STR_ISP2X_ISP_MIPI_DPHY_RX       "rockchip-mipi-dphy-rx"

#define MI_STR_ISP2X_MODEL_ISPP0            "rkispp0"
#define MI_STR_ISP2X_MODEL_ISPP1            "rkispp1"
#define MI_STR_ISP2X_MODEL_ISPP             "rkispp" // equal to rkispp0
#define MI_STR_ISP2X_ISPP_INPUT_IMAGE       "rkispp_input_image"
#define MI_STR_ISP2X_ISPP_M_BYPASS          "rkispp_m_bypass"
#define MI_STR_ISP2X_ISPP_SCALE0            "rkispp_scale0"
#define MI_STR_ISP2X_ISPP_SCALE1            "rkispp_scale1"
#define MI_STR_ISP2X_ISPP_SCALE2            "rkispp_scale2"
#define MI_STR_ISP2X_ISPP_INPUT_PARAMS      "rkispp_input_params"
#define MI_STR_ISP2X_ISPP_STATS             "rkispp-stats"
#define MI_STR_ISP2X_ISPP_SUBDEV            "rkispp-subdev"

#define MI_STR_VICAP_MODEL_CIF              "rkcif"
#define MI_STR_VICAP_MODEL_CIF_DVP          "rkcif_dvp"
#define MI_STR_VICAP_MODEL_MIPI_LVDS        "rkcif_mipi_lvds"
#define MI_STR_VICAP_MODEL_LITE_MIPI_LVDS   "rkcif_lite_mipi_lvds"
#define MI_STR_VICAP_STREAM_MIPI_ID0        "stream_cif_mipi_id0"
#define MI_STR_VICAP_STREAM_MIPI_ID1        "stream_cif_mipi_id1"
#define MI_STR_VICAP_STREAM_MIPI_ID2        "stream_cif_mipi_id2"
#define MI_STR_VICAP_STREAM_MIPI_ID3        "stream_cif_mipi_id3"
#define MI_STR_VICAP_STREAM_CIF             "stream_cif"
#define MI_STR_VICAP_MIPI_LUMA              "rkcif-mipi-luma"
#define MI_STR_VICAP_MIPI_CSI2              "rockchip-mipi-csi2"
#define MI_STR_VICAP_LVDS_SUBDEV            "rkcif-lvds-subdev"
#define MI_STR_VICAP_LITE_LVDS_SUBDEV       "rkcif-lite-lvds-subdev"
#define MI_STR_VICAP_MIPI_DPHY_RX           "rockchip-mipi-dphy-rx"

namespace RkCam {

static XCamReturn get_isp_ver(rk_aiq_isp_hw_info_t *hw_info) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    struct v4l2_capability cap;
    V4l2Device vdev(hw_info->isp_info[0].stats_path);


    ret = vdev.open();
    if (ret != XCAM_RETURN_NO_ERROR) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "failed to open dev (%s)", hw_info->isp_info[0].stats_path);
        return XCAM_RETURN_ERROR_FAILED;
    }

    if (vdev.query_cap(cap) == XCAM_RETURN_NO_ERROR) {
        char *p;
        p = strrchr((char*)cap.driver, '_');
        if (p == NULL) {
            ret = XCAM_RETURN_ERROR_FAILED;
            goto out;
        }

        if (*(p + 1) != 'v') {
            ret = XCAM_RETURN_ERROR_FAILED;
            goto out;
        }

        hw_info->hw_ver_info.isp_ver = atoi(p + 2);
        //awb/aec version?
        vdev.close();
        return XCAM_RETURN_NO_ERROR;
    } else {
        ret = XCAM_RETURN_ERROR_FAILED;
        goto out;
    }

out:
    vdev.close();
    LOGE_CAMHW_SUBM(CAMHW_INFO, "get isp version failed !");
    return ret;
}

static XCamReturn get_sensor_caps(rk_sensor_full_info_t *sensor_info) {
    struct v4l2_subdev_frame_size_enum fsize_enum;
    struct v4l2_subdev_mbus_code_enum  code_enum;
    std::vector<uint32_t> formats;
    rk_frame_fmt_t frameSize;
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    V4l2SubDevice vdev(sensor_info->device_name.c_str());
    ret = vdev.open();
    if (ret != XCAM_RETURN_NO_ERROR) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "failed to open dev (%s)", sensor_info->device_name.c_str());
        return XCAM_RETURN_ERROR_FAILED;
    }
    //get module info
    struct rkmodule_inf *minfo = &(sensor_info->mod_info);
    if(vdev.io_control(RKMODULE_GET_MODULE_INFO, minfo) < 0) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "@%s %s: Get sensor module info failed", __FUNCTION__, sensor_info->device_name.c_str());
        return XCAM_RETURN_ERROR_FAILED;
    }
    sensor_info->len_name = std::string(minfo->base.lens);

#if 0
    memset(&code_enum, 0, sizeof(code_enum));
    while (vdev.io_control(VIDIOC_SUBDEV_ENUM_MBUS_CODE, &code_enum) == 0) {
        formats.push_back(code_enum.code);
        code_enum.index++;
    };

    //TODO: sensor driver not supported now
    for (auto it = formats.begin(); it != formats.end(); ++it) {
        fsize_enum.pad = 0;
        fsize_enum.index = 0;
        fsize_enum.code = *it;
        while (vdev.io_control(VIDIOC_SUBDEV_ENUM_FRAME_SIZE, &fsize_enum) == 0) {
            frameSize.format = (rk_aiq_format_t)fsize_enum.code;
            frameSize.width = fsize_enum.max_width;
            frameSize.height = fsize_enum.max_height;
            sensor_info->frame_size.push_back(frameSize);
            fsize_enum.index++;
        };
    }
    if(!formats.size() || !sensor_info->frame_size.size()) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "@%s %s: Enum sensor frame size failed", __FUNCTION__, sensor_info->device_name.c_str());
        ret = XCAM_RETURN_ERROR_FAILED;
    }
#endif

    struct v4l2_subdev_frame_interval_enum fie;
    memset(&fie, 0, sizeof(fie));
    while(vdev.io_control(VIDIOC_SUBDEV_ENUM_FRAME_INTERVAL, &fie) == 0) {
        frameSize.format = (rk_aiq_format_t)fie.code;
        frameSize.width = fie.width;
        frameSize.height = fie.height;
        frameSize.fps = fie.interval.denominator / fie.interval.numerator;
        frameSize.hdr_mode = fie.reserved[0];
        sensor_info->frame_size.push_back(frameSize);
        fie.index++;
    }
    if (fie.index == 0)
        LOGE_CAMHW_SUBM(CAMHW_INFO, "@%s %s: Enum sensor frame interval failed", __FUNCTION__, sensor_info->device_name.c_str());
    vdev.close();

    return ret;
}

static XCamReturn
parse_module_info(rk_sensor_full_info_t *sensor_info)
{
    // sensor entity name format SHOULD be like this:
    // m00_b_ov13850 1-0010
    std::string entity_name(sensor_info->sensor_name);

    if (entity_name.empty())
        return XCAM_RETURN_ERROR_SENSOR;

    int parse_index = 0;

    if (entity_name.at(parse_index) != 'm') {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "%d:parse sensor entity name %s error at %d, please check sensor driver !",
                        __LINE__, entity_name.c_str(), parse_index);
        return XCAM_RETURN_ERROR_SENSOR;
    }

    std::string index_str = entity_name.substr (parse_index, 3);
    sensor_info->module_index_str = index_str;

    parse_index += 3;

    if (entity_name.at(parse_index) != '_') {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "%d:parse sensor entity name %s error at %d, please check sensor driver !",
                        __LINE__, entity_name.c_str(), parse_index);
        return XCAM_RETURN_ERROR_SENSOR;
    }

    parse_index++;

    if (entity_name.at(parse_index) != 'b' &&
            entity_name.at(parse_index) != 'f') {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "%d:parse sensor entity name %s error at %d, please check sensor driver !",
                        __LINE__, entity_name.c_str(), parse_index);
        return XCAM_RETURN_ERROR_SENSOR;
    }
    sensor_info->phy_module_orient = entity_name.at(parse_index);

    parse_index++;

    if (entity_name.at(parse_index) != '_') {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "%d:parse sensor entity name %s error at %d, please check sensor driver !",
                        __LINE__, entity_name.c_str(), parse_index);
        return XCAM_RETURN_ERROR_SENSOR;
    }

    parse_index++;

    std::size_t real_name_end = std::string::npos;
    if ((real_name_end = entity_name.find(' ')) == std::string::npos) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "%d:parse sensor entity name %s error at %d, please check sensor driver !",
                        __LINE__, entity_name.c_str(), parse_index);
        return XCAM_RETURN_ERROR_SENSOR;
    }

    std::string real_name_str = entity_name.substr(parse_index, real_name_end - parse_index);
    sensor_info->module_real_sensor_name = real_name_str;

    LOGD_CAMHW_SUBM(CAMHW_INFO, "%s:%d, real sensor name %s, module ori %c, module id %s",
                    __FUNCTION__, __LINE__, sensor_info->module_real_sensor_name.c_str(),
                    sensor_info->phy_module_orient, sensor_info->module_index_str.c_str());

    return XCAM_RETURN_NO_ERROR;
}

static rk_aiq_ispp_t*
get_ispp_subdevs(struct media_device *device, const char *devpath, rk_aiq_ispp_t* ispp_info)
{
    media_entity *entity = NULL;
    const char *entity_name = NULL;
    int index = 0;

    if(!device || !ispp_info || !devpath)
        return NULL;

    for(index = 0; index < 2; index++) {
        if (0 == strlen(ispp_info[index].media_dev_path))
            break;
        if (0 == strncmp(ispp_info[index].media_dev_path, devpath, sizeof(ispp_info[index].media_dev_path))) {
            LOGD_CAMHW_SUBM(CAMHW_INFO, "isp info of path %s exists!", devpath);
            return &ispp_info[index];
        }
    }

    if (index >= 2)
        return NULL;

    if (strcmp(device->info.model, "rkispp0") == 0 ||
            strcmp(device->info.model, "rkispp") == 0)
        ispp_info[index].model_idx = 0;
    else if (strcmp(device->info.model, "rkispp1") == 0)
        ispp_info[index].model_idx = 1;
    else
        ispp_info[index].model_idx = -1;

    strncpy(ispp_info[index].media_dev_path, devpath, sizeof(ispp_info[index].media_dev_path));

    entity = media_get_entity_by_name(device, "rkispp_input_image", strlen("rkispp_input_image"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_input_image_path, entity_name, sizeof(ispp_info[index].pp_input_image_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp_m_bypass", strlen("rkispp_m_bypass"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_m_bypass_path, entity_name, sizeof(ispp_info[index].pp_m_bypass_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp_scale0", strlen("rkispp_scale0"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_scale0_path, entity_name, sizeof(ispp_info[index].pp_scale0_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp_scale1", strlen("rkispp_scale1"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_scale1_path, entity_name, sizeof(ispp_info[index].pp_scale1_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp_scale2", strlen("rkispp_scale2"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_scale2_path, entity_name, sizeof(ispp_info[index].pp_scale2_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp_input_params", strlen("rkispp_input_params"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_input_params_path, entity_name, sizeof(ispp_info[index].pp_input_params_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp-stats", strlen("rkispp-stats"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_stats_path, entity_name, sizeof(ispp_info[index].pp_stats_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkispp-subdev", strlen("rkispp-subdev"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(ispp_info[index].pp_dev_path, entity_name, sizeof(ispp_info[index].pp_dev_path));
        }
    }

    LOGI_CAMHW_SUBM(CAMHW_INFO, "model(%s): ispp_info(%d): ispp-subdev entity name: %s\n",
                    device->info.model, index,
                    ispp_info[index].pp_dev_path);

    return &ispp_info[index];
}

static rk_aiq_isp_t*
get_isp_subdevs(struct media_device *device, const char *devpath, rk_aiq_isp_t* isp_info)
{
    media_entity *entity = NULL;
    const char *entity_name = NULL;
    int index = 0;
    if(!device || !isp_info || !devpath)
        return NULL;

    for(index = 0; index < 2; index++) {
        if (0 == strlen(isp_info[index].media_dev_path))
            break;
        if (0 == strncmp(isp_info[index].media_dev_path, devpath, sizeof(isp_info[index].media_dev_path))) {
            LOGD_CAMHW_SUBM(CAMHW_INFO, "isp info of path %s exists!", devpath);
            return &isp_info[index];
        }
    }
    if (index >= 2)
        return NULL;

    if (strcmp(device->info.model, "rkisp0") == 0 ||
            strcmp(device->info.model, "rkisp") == 0)
        isp_info[index].model_idx = 0;
    else if (strcmp(device->info.model, "rkisp1") == 0)
        isp_info[index].model_idx = 1;
    else
        isp_info[index].model_idx = -1;

    strncpy(isp_info[index].media_dev_path, devpath, sizeof(isp_info[index].media_dev_path));

    entity = media_get_entity_by_name(device, "rkisp-isp-subdev", strlen("rkisp-isp-subdev"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].isp_dev_path, entity_name, sizeof(isp_info[index].isp_dev_path));
        }
    }

    entity = media_get_entity_by_name(device, "rkisp-csi-subdev", strlen("rkisp-csi-subdev"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].csi_dev_path, entity_name, sizeof(isp_info[index].csi_dev_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp-mpfbc-subdev", strlen("rkisp-mpfbc-subdev"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].mpfbc_dev_path, entity_name, sizeof(isp_info[index].mpfbc_dev_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_mainpath", strlen("rkisp_mainpath"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].main_path, entity_name, sizeof(isp_info[index].main_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_selfpath", strlen("rkisp_selfpath"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].self_path, entity_name, sizeof(isp_info[index].self_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawwr0", strlen("rkisp_rawwr0"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawwr0_path, entity_name, sizeof(isp_info[index].rawwr0_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawwr1", strlen("rkisp_rawwr1"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawwr1_path, entity_name, sizeof(isp_info[index].rawwr1_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawwr2", strlen("rkisp_rawwr2"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawwr2_path, entity_name, sizeof(isp_info[index].rawwr2_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawwr3", strlen("rkisp_rawwr3"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawwr3_path, entity_name, sizeof(isp_info[index].rawwr3_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_dmapath", strlen("rkisp_dmapath"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].dma_path, entity_name, sizeof(isp_info[index].dma_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawrd0_m", strlen("rkisp_rawrd0_m"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawrd0_m_path, entity_name, sizeof(isp_info[index].rawrd0_m_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawrd1_l", strlen("rkisp_rawrd1_l"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawrd1_l_path, entity_name, sizeof(isp_info[index].rawrd1_l_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp_rawrd2_s", strlen("rkisp_rawrd2_s"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].rawrd2_s_path, entity_name, sizeof(isp_info[index].rawrd2_s_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp-statistics", strlen("rkisp-statistics"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].stats_path, entity_name, sizeof(isp_info[index].stats_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp-input-params", strlen("rkisp-input-params"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].input_params_path, entity_name, sizeof(isp_info[index].input_params_path));
        }
    }
    entity = media_get_entity_by_name(device, "rkisp-mipi-luma", strlen("rkisp-mipi-luma"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].mipi_luma_path, entity_name, sizeof(isp_info[index].mipi_luma_path));
        }
    }
    entity = media_get_entity_by_name(device, "rockchip-mipi-dphy-rx", strlen("rockchip-mipi-dphy-rx"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(isp_info[index].mipi_dphy_rx_path, entity_name, sizeof(isp_info[index].mipi_dphy_rx_path));
        }
    }


    LOGI_CAMHW_SUBM(CAMHW_INFO, "model(%s): isp_info(%d): ispp-subdev entity name: %s\n",
                    device->info.model, index,
                    isp_info[index].isp_dev_path);

    return &isp_info[index];
}

static rk_aiq_cif_info_t*
get_cif_subdevs(struct media_device *device, const char *devpath, rk_aiq_cif_info_t* cif_info)
{
    media_entity *entity = NULL;
    const char *entity_name = NULL;
    int index = 0;
    if(!device || !devpath || !cif_info)
        return NULL;

    for(index = 0; index < MAX_CIF_NUM; index++) {
        if (0 == strlen(cif_info[index].media_dev_path))
            break;
        if (0 == strncmp(cif_info[index].media_dev_path, devpath, sizeof(cif_info[index].media_dev_path))) {
            LOGD_CAMHW_SUBM(CAMHW_INFO, "isp info of path %s exists!", devpath);
            return &cif_info[index];
        }
    }
    if (index >= 2)
        return NULL;

    cif_info[index].model_idx = index;

    strncpy(cif_info[index].media_dev_path, devpath, sizeof(cif_info[index].media_dev_path));

    entity = media_get_entity_by_name(device, "stream_cif_mipi_id0", strlen("stream_cif_mipi_id0"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_id0, entity_name, sizeof(cif_info[index].mipi_id0));
        }
    }

    entity = media_get_entity_by_name(device, "stream_cif_mipi_id1", strlen("stream_cif_mipi_id1"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_id1, entity_name, sizeof(cif_info[index].mipi_id1));
        }
    }

    entity = media_get_entity_by_name(device, "stream_cif_mipi_id2", strlen("stream_cif_mipi_id2"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_id2, entity_name, sizeof(cif_info[index].mipi_id2));
        }
    }

    entity = media_get_entity_by_name(device, "stream_cif_mipi_id3", strlen("stream_cif_mipi_id3"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_id3, entity_name, sizeof(cif_info[index].mipi_id3));
        }
    }

    entity = media_get_entity_by_name(device, "rkcif-mipi-luma", strlen("rkisp-mipi-luma"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_luma_path, entity_name, sizeof(cif_info[index].mipi_luma_path));
        }
    }

    entity = media_get_entity_by_name(device, "rockchip-mipi-csi2", strlen("rockchip-mipi-csi2"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_csi2_sd_path, entity_name, sizeof(cif_info[index].mipi_csi2_sd_path));
        }
    }

    entity = media_get_entity_by_name(device, "rkcif-lvds-subdev", strlen("rkcif-lvds-subdev"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].lvds_sd_path, entity_name, sizeof(cif_info[index].lvds_sd_path));
        }
    }

    entity = media_get_entity_by_name(device, "rkcif-lite-lvds-subdev", strlen("rkcif-lite-lvds-subdev"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].lvds_sd_path, entity_name, sizeof(cif_info[index].lvds_sd_path));
        }
    }

    entity = media_get_entity_by_name(device, "rockchip-mipi-dphy-rx", strlen("rockchip-mipi-dphy-rx"));
    if(entity) {
        entity_name = media_entity_get_devname (entity);
        if(entity_name) {
            strncpy(cif_info[index].mipi_dphy_rx_path, entity_name, sizeof(cif_info[index].mipi_csi2_sd_path));
        }
    }

    return &cif_info[index];
}


static
XCamReturn
SensorInfoCopy(rk_sensor_full_info_t *finfo, rk_aiq_static_info_t *info) {
    int fs_num, i = 0;
    rk_aiq_sensor_info_t *sinfo = NULL;

    //info->media_node_index = finfo->media_node_index;
    strncpy(info->lens_info.len_name, finfo->len_name.c_str(), sizeof(info->lens_info.len_name));
    sinfo = &info->sensor_info;
    strncpy(sinfo->sensor_name, finfo->sensor_name.c_str(), sizeof(sinfo->sensor_name));
    fs_num = finfo->frame_size.size();
    if (fs_num) {
        for (auto iter = finfo->frame_size.begin(); iter != finfo->frame_size.end() && i < 10; ++iter, i++) {
            sinfo->support_fmt[i].width = (*iter).width;
            sinfo->support_fmt[i].height = (*iter).height;
            sinfo->support_fmt[i].format = (*iter).format;
            sinfo->support_fmt[i].fps = (*iter).fps;
            sinfo->support_fmt[i].hdr_mode = (*iter).hdr_mode;
        }
        sinfo->num = i;
    }

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
CamHwInfo::selectIqFile(const char* sns_ent_name, char* iqfile_name)
{
    if (!sns_ent_name || !iqfile_name)
        return XCAM_RETURN_ERROR_SENSOR;
    std::map<std::string, SmartPtr<rk_sensor_full_info_t>>::iterator it;
    const struct rkmodule_base_inf* base_inf = NULL;
    const char *sensor_name, *module_name, *lens_name;
    char sensor_name_full[32];
    std::string str(sns_ent_name);

    if ((it = CamHwInfo::mSensorHwInfos.find(str)) == CamHwInfo::mSensorHwInfos.end()) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "can't find sensor %s", sns_ent_name);
        return XCAM_RETURN_ERROR_SENSOR;
    }
    base_inf = &(it->second.ptr()->mod_info.base);
    if (!strlen(base_inf->module) || !strlen(base_inf->sensor) ||
            !strlen(base_inf->lens)) {
        LOGE_CAMHW_SUBM(CAMHW_INFO, "no camera module info, check the drv !");
        return XCAM_RETURN_ERROR_SENSOR;
    }
    sensor_name = base_inf->sensor;
    strncpy(sensor_name_full, sensor_name, 32);

    module_name = base_inf->module;
    lens_name = base_inf->lens;
    sprintf(iqfile_name, "%s_%s_%s.xml",
            sensor_name_full, module_name, lens_name);
    return XCAM_RETURN_NO_ERROR;
}

rk_aiq_static_info_t*
CamHwInfo::getStaticCamHwInfo(const char* sns_ent_name, uint16_t index)
{
    std::map<std::string, SmartPtr<rk_aiq_static_info_t>>::iterator it;

    if (sns_ent_name) {
        std::string str(sns_ent_name);

        it = mCamHwInfos.find(str);
        if (it != mCamHwInfos.end()) {
            LOGD_CAMHW_SUBM(CAMHW_INFO, "find camerainfo of %s!", sns_ent_name);
            return it->second.ptr();
        } else {
            LOGE_CAMHW_SUBM(CAMHW_INFO, "camerainfo of %s not fount!", sns_ent_name);
        }
    } else {
        if (index >= 0 && index < mCamHwInfos.size()) {
            int i = 0;
            for (it = mCamHwInfos.begin(); it != mCamHwInfos.end(); it++, i++) {
                if (i == index)
                    return it->second.ptr();
            }
        }
    }

    return NULL;
}

XCamReturn
CamHwInfo::clearStaticCamHwInfo()
{
    /* std::map<std::string, SmartPtr<rk_aiq_static_info_t>>::iterator it1; */
    /* std::map<std::string, SmartPtr<rk_sensor_full_info_t>>::iterator it2; */

    /* for (it1 = mCamHwInfos.begin(); it1 != mCamHwInfos.end(); it1++) { */
    /*     rk_aiq_static_info_t *ptr = it1->second.ptr(); */
    /*     delete ptr; */
    /* } */
    /* for (it2 = mSensorHwInfos.begin(); it2 != mSensorHwInfos.end(); it2++) { */
    /*     rk_sensor_full_info_t *ptr = it2->second.ptr(); */
    /*     delete ptr; */
    /* } */
    mCamHwInfos.clear();
    mSensorHwInfos.clear();

    return XCAM_RETURN_NO_ERROR;
}

void
CamHwInfo::findAttachedSubdevs(struct media_device *device, uint32_t count, rk_sensor_full_info_t *s_info)
{
    const struct media_entity_desc *entity_info = NULL;
    struct media_entity *entity = NULL;
    uint32_t k;

    for (k = 0; k < count; ++k) {
        entity = media_get_entity (device, k);
        entity_info = media_entity_get_info(entity);
        if ((NULL != entity_info) && (entity_info->type == MEDIA_ENT_T_V4L2_SUBDEV_LENS)) {
            if ((entity_info->name[0] == 'm') &&
                    (strncmp(entity_info->name, s_info->module_index_str.c_str(), 3) == 0)) {
                if (entity_info->flags == 1)
                    s_info->module_ircut_dev_name = std::string(media_entity_get_devname(entity));
                else//vcm
                    s_info->module_lens_dev_name = std::string(media_entity_get_devname(entity));
            }
        } else if ((NULL != entity_info) && (entity_info->type == MEDIA_ENT_T_V4L2_SUBDEV_FLASH)) {
            if ((entity_info->name[0] == 'm') &&
                    (strncmp(entity_info->name, s_info->module_index_str.c_str(), 3) == 0)) {

                /* check if entity name has the format string mxx_x_xxx-irxxx */
                if (strstr(entity_info->name, "-ir") != NULL) {
                    s_info->module_flash_ir_dev_name[s_info->flash_ir_num++] =
                        std::string(media_entity_get_devname(entity));
                } else
                    s_info->module_flash_dev_name[s_info->flash_num++] =
                        std::string(media_entity_get_devname(entity));
            }
        }
    }
    // query flash infos
    if (s_info->flash_num) {
        SmartPtr<FlashLightHw> fl = new FlashLightHw(s_info->module_flash_dev_name, s_info->flash_num);
        fl->init(1);
        s_info->fl_strth_adj_sup = fl->isStrengthAdj();
        fl->deinit();
    }
    if (s_info->flash_ir_num) {
        SmartPtr<FlashLightHw> fl_ir = new FlashLightHw(s_info->module_flash_ir_dev_name, s_info->flash_ir_num);
        fl_ir->init(1);
        s_info->fl_ir_strth_adj_sup = fl_ir->isStrengthAdj();
        fl_ir->deinit();
    }
}

XCamReturn
CamHwInfo::initCamHwInfos()
{
    char sys_path[64], devpath[32];
    FILE *fp = NULL;
    struct media_device *device = NULL;
    uint32_t nents, j = 0, i = 0, node_index = 0;
    const struct media_entity_desc *entity_info = NULL;
    struct media_entity *entity = NULL;

    while (i < MAX_MEDIA_INDEX) {
        node_index = i;
        snprintf (sys_path, 64, "/dev/media%d", i++);
        fp = fopen (sys_path, "r");
        if (!fp)
            continue;
        fclose (fp);
        device = media_device_new (sys_path);

        /* Enumerate entities, pads and links. */
        media_device_enumerate (device);

        rk_aiq_isp_t* isp_info = NULL;
        rk_aiq_cif_info_t* cif_info = NULL;
        if (strcmp(device->info.model, "rkispp0") == 0 ||
                strcmp(device->info.model, "rkispp1") == 0 ||
                strcmp(device->info.model, "rkispp") == 0) {
            get_ispp_subdevs(device, sys_path, CamHwInfo::mIspHwInfos.ispp_info);
            goto media_unref;
        } else if (strcmp(device->info.model, "rkisp0") == 0 ||
                   strcmp(device->info.model, "rkisp1") == 0 ||
                   strcmp(device->info.model, "rkisp") == 0) {
            isp_info = get_isp_subdevs(device, sys_path, CamHwInfo::mIspHwInfos.isp_info);
        } else if (strcmp(device->info.model, "rkcif") == 0 ||
                   strcmp(device->info.model, "rkcif_mipi_lvds") == 0 ||
                   strcmp(device->info.model, "rkcif_lite_mipi_lvds") == 0) {
            cif_info = get_cif_subdevs(device, sys_path, CamHwInfo::mCifHwInfos.cif_info);
        } else {
            goto media_unref;
        }

        nents = media_get_entities_count (device);
        for (j = 0; j < nents; ++j) {
            entity = media_get_entity (device, j);
            entity_info = media_entity_get_info(entity);
            if ((NULL != entity_info) && (entity_info->type == MEDIA_ENT_T_V4L2_SUBDEV_SENSOR)) {
                rk_aiq_static_info_t *info = new rk_aiq_static_info_t();
                rk_sensor_full_info_t *s_full_info = new rk_sensor_full_info_t();
                s_full_info->media_node_index = node_index;
                strncpy(devpath, media_entity_get_devname(entity), sizeof(devpath));
                s_full_info->device_name = std::string(devpath);
                s_full_info->sensor_name = std::string(entity_info->name);
                s_full_info->parent_media_dev = std::string(sys_path);
                parse_module_info(s_full_info);
                get_sensor_caps(s_full_info);

                if (cif_info) {
                    s_full_info->linked_to_isp = false;
                    s_full_info->cif_info = cif_info;
                    s_full_info->isp_info = NULL;
                } else if (isp_info) {
                    s_full_info->linked_to_isp = true;
                    isp_info->linked_sensor = true;
                    s_full_info->isp_info = isp_info;
                } else {
                    LOGE_CAMHW_SUBM(CAMHW_INFO, "sensor device mount error!\n");
                }

                findAttachedSubdevs(device, nents, s_full_info);
                SensorInfoCopy(s_full_info, info);
                info->has_lens_vcm = s_full_info->module_lens_dev_name.empty() ? false : true;
                info->has_fl = s_full_info->flash_num > 0 ? true : false;
                info->has_irc = s_full_info->module_ircut_dev_name.empty() ? false : true;
                info->fl_strth_adj_sup = s_full_info->fl_ir_strth_adj_sup;
                info->fl_ir_strth_adj_sup = s_full_info->fl_ir_strth_adj_sup;
                CamHwInfo::mSensorHwInfos[s_full_info->sensor_name] = s_full_info;
                CamHwInfo::mCamHwInfos[s_full_info->sensor_name] = info;
            }
        }

media_unref:
        media_device_unref (device);
    }

    std::map<std::string, SmartPtr<rk_sensor_full_info_t>>::iterator iter;
    for(iter = CamHwInfo::mSensorHwInfos.begin(); \
            iter != CamHwInfo::mSensorHwInfos.end(); iter++) {
        LOGI_CAMHW_SUBM(CAMHW_INFO, "match the sensor_name(%s) media link\n", (iter->first).c_str());
        SmartPtr<rk_sensor_full_info_t> s_full_info = iter->second;

        /*
         * The ISP and ISPP match links through the media device model
         */
        if (s_full_info->linked_to_isp) {
            for (i = 0; i < 2; i++) {
                LOGI_CAMHW_SUBM(CAMHW_INFO, "isp model_idx: %d, ispp(%d) model_idx: %d\n",
                                s_full_info->isp_info->model_idx,
                                i,
                                CamHwInfo::mIspHwInfos.ispp_info[i].model_idx);

                if (s_full_info->isp_info->model_idx == CamHwInfo::mIspHwInfos.ispp_info[i].model_idx) {
                    s_full_info->ispp_info = &CamHwInfo::mIspHwInfos.ispp_info[i];
                    LOGI_CAMHW_SUBM(CAMHW_INFO, "isp(%d) link to ispp(%d)\n",
                                    s_full_info->isp_info->model_idx,
                                    CamHwInfo::mIspHwInfos.ispp_info[i].model_idx);
                    CamHwInfo::mCamHwInfos[s_full_info->sensor_name]->sensor_info.binded_strm_media_idx =
                        atoi(s_full_info->ispp_info->media_dev_path + strlen("/dev/media"));
                    LOGI_CAMHW_SUBM(CAMHW_INFO, "sensor %s adapted to pp media %d:%s\n",
                                    s_full_info->sensor_name.c_str(),
                                    CamHwInfo::mCamHwInfos[s_full_info->sensor_name]->sensor_info.binded_strm_media_idx,
                                    s_full_info->ispp_info->media_dev_path);
                    break;
                }
            }
        } else {
            /*
             * Determine which isp that vipCap is linked
             */
            for (i = 0; i < 2; i++) {
                if (!CamHwInfo::mIspHwInfos.isp_info[i].linked_sensor) {
                    s_full_info->isp_info = &CamHwInfo::mIspHwInfos.isp_info[i];
                    s_full_info->ispp_info = &CamHwInfo::mIspHwInfos.ispp_info[i];
                    LOGI_CAMHW_SUBM(CAMHW_INFO, "vicap link to isp(%d) to ispp(%d)\n",
                                    s_full_info->isp_info->model_idx,
                                    s_full_info->ispp_info->model_idx);
                    CamHwInfo::mCamHwInfos[s_full_info->sensor_name]->sensor_info.binded_strm_media_idx =
                        atoi(s_full_info->ispp_info->media_dev_path + strlen("/dev/media"));
                    LOGI_CAMHW_SUBM(CAMHW_INFO, "sensor %s adapted to pp media %d:%s\n",
                                    s_full_info->sensor_name.c_str(),
                                    CamHwInfo::mCamHwInfos[s_full_info->sensor_name]->sensor_info.binded_strm_media_idx,
                                    s_full_info->ispp_info->media_dev_path);
                    CamHwInfo::mIspHwInfos.isp_info[i].linked_sensor = true;
                    break;
                }
            }
        }

        if (!s_full_info->isp_info || !s_full_info->ispp_info) {
            LOGE_CAMHW_SUBM(CAMHW_INFO, "get isp or ispp info fail, something gos wrong!");
        } else {
            //CamHwInfo::mCamHwInfos[s_full_info->sensor_name]->linked_isp_info = *s_full_info->isp_info;
            //CamHwInfo::mCamHwInfos[s_full_info->sensor_name]->linked_ispp_info = *s_full_info->ispp_info;
        }
    }

    get_isp_ver(&CamHwInfo::mIspHwInfos);
    return XCAM_RETURN_NO_ERROR;
}

const char*
CamHwInfo::getBindedSnsEntNmByVd(const char* vd)
{
    if (!vd)
        return NULL;

    std::map<std::string, SmartPtr<rk_sensor_full_info_t>>::iterator iter;
    for(iter = CamHwInfo::mSensorHwInfos.begin(); \
            iter != CamHwInfo::mSensorHwInfos.end(); iter++) {
        SmartPtr<rk_sensor_full_info_t> s_full_info = iter->second;
        if (strstr(s_full_info->ispp_info->pp_m_bypass_path, vd) ||
                strstr(s_full_info->ispp_info->pp_scale0_path, vd) ||
                strstr(s_full_info->ispp_info->pp_scale1_path, vd) ||
                strstr(s_full_info->ispp_info->pp_scale2_path, vd))
            return  s_full_info->sensor_name.c_str();
    }

    return NULL;
}

};
