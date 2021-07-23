
if(NOT CMAKE_BUILD_TYPE)
    # default to Release build for GCC builds
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

cmake_minimum_required(VERSION 2.8)

#project name
PROJECT(rk_aiq)

# ----------------------------------------------------------------------------
# set property to classify library kinds
# ----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#set BUILROOT_BUILD_PROJECT
set(BUILROOT_BUILD_PROJECT true)
#set(ENABLE_ASAN true)

#set RK_SIMULATOR_HW
#set(RK_SIMULATOR_HW true)

if(NOT RK_SIMULATOR_HW)
	set(GENMESH_ONLINE true)
endif()

# ----------------------------------------------------------------------------
# SET THE COMPILER PATH
# ----------------------------------------------------------------------------
if(RK_SIMULATOR_HW)
	add_compile_options(-DRK_SIMULATOR_HW)
elseif(BUILROOT_BUILD_PROJECT)
	#add_compile_options(-DDISABLE_PP)
	#add_compile_options(-DDISABLE_PP_STATS)
	if(ARCH STREQUAL "aarch64")
		SET(CMAKE_C_COMPILER "/home/camera/camera/rv1109_sdk/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc")
		SET(CMAKE_CXX_COMPILER "/home/camera/camera/rv1109_sdk/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++")
	else()
		SET(CMAKE_C_COMPILER "/home/camera/camera/rv1109_sdk/buildroot/output/rockchip_puma/host/bin/arm-linux-gnueabihf-gcc")
		SET(CMAKE_CXX_COMPILER "/home/camera/camera/rv1109_sdk/buildroot/output/rockchip_puma/host/bin/arm-linux-gnueabihf-g++")
	endif()
else()
    SET(CMAKE_C_COMPILER "/home/hkj/isp2-fpga/fpga-ramdisk/toolchains/host/bin/arm-buildroot-linux-gnueabi-gcc")
    SET(CMAKE_CXX_COMPILER "/home/hkj/isp2-fpga/fpga-ramdisk/toolchains/host/bin/arm-buildroot-linux-gnueabi-g++")
endif()

# ----------------------------------------------------------------------------
# ADD SYMBOL TABLE FOR GDB
# ----------------------------------------------------------------------------
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -Wno-reorder -Werror")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -Werror -Wno-reorder")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#head file path
include_directories(
xcore
xcore/base
aiq_core
algos
common
common/linux
hwi
ipc
iq_parser
uAPI
#algos/awb
#../core/inc/luma
#../core/inc/stat_3a_ae
#../core/inc/stat_3a_af
#../core/inc/orb
#../core/inc/common
./
./common/
include
include/common
include/common/mediactl
include/iq_parser
include/uAPI
include/xcore
include/xcore/base
include/algos
include/algos/a3dlut
include/algos/ablc
include/algos/accm
include/algos/acgc
include/algos/acp
include/algos/adebayer
include/algos/adehaze
include/algos/adpcc
include/algos/ae
include/algos/af
include/algos/afec
include/algos/agamma
include/algos/agic
include/algos/ahdr
include/algos/adrc
include/algos/aie
include/algos/aldch
include/algos/alsc
include/algos/anr
include/algos/aorb
include/algos/ar2y
include/algos/asd
include/algos/asharp
include/algos/awb
include/algos/awdr
include/algos/asharp3
include/algos/aynr2
include/algos/acnr
include/common/gen_mesh
)
#source directory
#AUX_SOURCE_DIRECTORY(demo/src DIR_SRCS)
set (AIQ_CORE_SRC
	aiq_core/RkAiqHandle.cpp
	aiq_core/RkAiqHandleInt.cpp
	aiq_core/RkAiqHandleIntV21.cpp
	aiq_core/RkAiqCore.cpp
	aiq_core/RkAiqCoreV21.cpp
	aiq_core/RkLumaCore.cpp
	)
set (ALGO_COMMON_SRC
	algos/interpolation.cpp
	)
if(GENMESH_ONLINE)
set (COMMON_SRC
    common/mediactl/mediactl.c
    common/gen_mesh/genMesh.cpp
    )
else()
set (COMMON_SRC
	common/mediactl/mediactl.c
	)
endif()
if(RK_SIMULATOR_HW)
set (HWI_SRC
        hwi/CamHwBase.cpp
        hwi/simulator/CamHwSimulator.cpp
        )
else()
set (HWI_SRC
        hwi/CamHwBase.cpp
        hwi/isp20/CamHwIsp20.cpp
        hwi/isp20/Isp20Params.cpp
        hwi/isp20/Isp20PollThread.cpp
        hwi/isp20/Isp20StatsBuffer.cpp
        hwi/isp20/Isp20_module_dbg.cpp
        hwi/isp20/Isp20OfflineFrmRead.cpp
		#hwi/isp21/Isp21Params.cpp
        hwi/isp21/CamHwIsp21.cpp
        hwi/SensorHw.cpp
        hwi/LensHw.cpp
        hwi/FlashLight.cpp
        )
endif()
set (IPC_SRC
	)
set (IQ_PARSER_SRC
	iq_parser/RkAiqCalibParser.cpp
	iq_parser/RkAiqCalibDb.cpp
	iq_parser/RkAiqCalibTag.cpp
	iq_parser/tinyxml2.cpp
	iq_parser/xmltags.cpp
	iq_parser/RkAiqCalibApi.cpp
	)
set (UAPI_SRC
	uAPI/rk_aiq_user_api_sysctl.cpp
	uAPI/rk_aiq_user_api_debug.cpp
	)
set (XCORE_SRC
	xcore/xcam_common.cpp
	xcore/xcam_log.cpp
	xcore/xcam_thread.cpp
	xcore/xcam_buffer.cpp
	xcore/video_buffer.cpp
	xcore/v4l2_buffer_proxy.cpp
	xcore/v4l2_device.cpp
	xcore/buffer_pool.cpp
	xcore/poll_thread.cpp
	)
set (RK_AIQ_SRC
	${ALGO_COMMON_SRC}
	${COMMON_SRC}
	${HWI_SRC}
	${IPC_SRC}
	${IQ_PARSER_SRC}
	${UAPI_SRC}
	${XCORE_SRC}
	${AIQ_CORE_SRC}
	RkAiqManager.cpp
    )
if(OPENCV_SUPPORT)
set (OPENCV_LIBS
    opencv_calib3d
    opencv_core
    opencv_dnn
    opencv_features2d
    opencv_flann
    opencv_gapi
    opencv_highgui
    opencv_imgcodecs
    opencv_imgproc
    opencv_ml
    opencv_objdetect
    opencv_photo
    opencv_stitching
    opencv_video
    opencv_videoio
    )
set (OPENCV_3RDPARTY_LIBS
    dl
    ade
    IlmImf
    ippicv
    ippiw
    ittnotify
    libprotobuf
    libwebp
    quirc
    )
set (OPENCV_LIBS_DIR
    ${PROJECT_SOURCE_DIR}/common/opencv2/lib
    ${PROJECT_SOURCE_DIR}/common/opencv2/lib/3rdparty
    )
add_compile_options(-DOPENCV_SUPPORT)
endif()

if(GENMESH_ONLINE)
    set (GENMESH_LIBS genMeshLib)
	if(ARCH STREQUAL "aarch64")
		set (GENMESH_LIBS_DIR
			${PROJECT_SOURCE_DIR}/common/gen_mesh/linux/genMesh_static_64bit/
			)
	else()
		set (GENMESH_LIBS_DIR
			${PROJECT_SOURCE_DIR}/common/gen_mesh/linux/genMesh_static_32bit/
			)
	endif()
    add_compile_options(-DGENMESH_ONLINE)
endif()

# set output binary path
if(WIN32)
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/all_lib/)
    SET(CMAKE_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
else()
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe/${CMAKE_BUILD_TYPE})
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/all_lib/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH})
endif()

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
#set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
message(${CMAKE_LIBRARY_PATH})
message(${LIBRARY_OUTPUT_PATH})

if(ENABLE_ASAN)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
SET(LINK_ASAN asan)
endif()

# add C++11 option
#add_compile_options(-std=c++11)
add_compile_options(-fPIC -Wall -Wno-error -Os -g)
add_compile_options(-Wno-unused-variable
					-Wno-unused-but-set-variable
					-Wno-unused-function
					-Wno-misleading-indentation
					-Wno-unused-label
					-fno-strict-aliasing)
# add_compile_options(-v)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# ----------------------------------------------------------------------------
# add algos implement
# ----------------------------------------------------------------------------
add_subdirectory(algos)

# surpress link_directories warning
cmake_policy(SET CMP0015 NEW)

#
link_directories("${LIBRARY_OUTPUT_PATH}" "${OPENCV_LIBS_DIR}" "${GENMESH_LIBS_DIR}")

add_library(rkaiq SHARED ${RK_AIQ_SRC})
set_target_properties(rkaiq PROPERTIES FOLDER "rk_aiq")
set_target_properties(rkaiq PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(rkaiq PROPERTIES C_VISIBILITY_PRESET default)
set_target_properties(rkaiq PROPERTIES CXX_VISIBILITY_PRESET default)

#
link_directories("${LIBRARY_OUTPUT_PATH}")

target_link_libraries(rkaiq rkaiq_ae rkaiq_awb rkaiq_af rkaiq_anr rkaiq_asd rkaiq_ahdr
	rkaiq_acp rkaiq_asharp rkaiq_adhaz rkaiq_a3dlut rkaiq_ablc rkaiq_accm rkaiq_acgc
	rkaiq_adebayer rkaiq_adpcc rkaiq_afec rkaiq_agamma rkaiq_agic rkaiq_aie rkaiq_aldch
	rkaiq_alsc rkaiq_aorb rkaiq_ar2y rkaiq_awdr rkaiq_adrc rkaiq_asharp_v3 rkaiq_aynr_v2 rkaiq_acnr_v1 rkaiq_arawnr_v2 ${OPENCV_LIBS}
	${OPENCV_3RDPARTY_LIBS} ${GENMESH_LIBS} ${LINK_ASAN} pthread)

set (VERSIONFILEPATH ${PROJECT_SOURCE_DIR}/)
execute_process(COMMAND sh
	${PROJECT_SOURCE_DIR}/tools/check-version-with-tuningtools.sh
	${VERSIONFILEPATH})
