/*
 * 
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 * PROPRIETARY/CONFIDENTIAL
 * 
 * This software is the confidential and proprietary information of SAMSUNG 
 * ELECTRONICS ("Confidential Information"). You agree and acknowledge that 
 * this software is owned by Samsung and you shall not disclose such 
 * Confidential Information and shall use it only in accordance with the terms 
 * of the license agreement you entered into with SAMSUNG ELECTRONICS. SAMSUNG 
 * make no representations or warranties about the suitability of the software, 
 * either express or implied, including but not limited to the implied 
 * warranties of merchantability, fitness for a particular purpose, or 
 * non-infringement. SAMSUNG shall not be liable for any damages suffered by 
 * licensee arising out of or related to this software.
 * 
 */
#include <stdio.h>
#include <tet_api.h>
#include <haptic.h>

#define API_NAME_HAPTIC_PLAY_FILE "haptic_vibrate_file" 
#define API_NAME_PLAY_MONOTONE "haptic_vibrate_monotone"
#define API_NAME_STOP_PLAY "haptic_stop_device"
#define API_NAME_PLAY_PTN "haptic_play_pattern"
#define API_NAME_STOP_PTN "haptic_stop_pattern"
#define TEST_IVT "/usr/share/immersion/01_Touch/touch_20ms_sharp.ivt"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_system_haptic_vibrate_monotone_p(void);
static void utc_system_haptic_vibrate_monotone_n_1(void);
static void utc_system_haptic_vibrate_monotone_n_2(void);
static void utc_system_haptic_stop_device_p(void);
static void utc_system_haptic_stop_device_n(void);

static void utc_system_haptic_vibrate_p(void);
static void utc_system_haptic_vibrate_n(void);

static void utc_system_haptic_stop_pattern_p(void);
static void utc_system_haptic_stop_pattern_n(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
    { utc_system_haptic_vibrate_monotone_p, POSITIVE_TC_IDX },
    { utc_system_haptic_vibrate_monotone_n_1, NEGATIVE_TC_IDX },
    { utc_system_haptic_vibrate_monotone_n_2, NEGATIVE_TC_IDX },
	{ utc_system_haptic_stop_device_p, POSITIVE_TC_IDX },
	{ utc_system_haptic_stop_device_n, NEGATIVE_TC_IDX },
    { utc_system_haptic_vibrate_p, POSITIVE_TC_IDX},
    { utc_system_haptic_vibrate_n, NEGATIVE_TC_IDX},
    { utc_system_haptic_stop_pattern_p, POSITIVE_TC_IDX},
    { utc_system_haptic_stop_pattern_n, NEGATIVE_TC_IDX},
	{ NULL, 0},
};


static void startup(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_initialize();

    dts_check_eq("haptic_initialize", error, HAPTIC_ERROR_NONE, "fail initialize in startup");
}

static void cleanup(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_deinitialize();

    dts_check_eq("haptic_deinitialize", error, HAPTIC_ERROR_NONE, "fail deinitialize in startup");
}


/**
 * @brief Positive test case of haptic_vibrate_monotone()
 */
static void utc_system_haptic_vibrate_monotone_p(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_vibrate_monotone(0, 1000, 50);
    if(error != HAPTIC_ERROR_NONE)
        dts_fail(API_NAME_PLAY_MONOTONE);

    dts_pass(API_NAME_PLAY_MONOTONE);
}

/**
 * @brief Negative test case of haptic_vibrate_monotone()
 */
static void utc_system_haptic_vibrate_monotone_n_1(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_vibrate_monotone(-1, 1000, 50);
    if(error == HAPTIC_ERROR_NONE)
        dts_fail(API_NAME_PLAY_MONOTONE);

    dts_pass(API_NAME_PLAY_MONOTONE);
}

/**
 * @brief Negative test case of haptic_vibrate_monotone()
 */
static void utc_system_haptic_vibrate_monotone_n_2(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_vibrate_monotone(0, -1, 50);
    if(error == HAPTIC_ERROR_NONE)
        dts_fail(API_NAME_PLAY_MONOTONE);

    dts_pass(API_NAME_PLAY_MONOTONE);
}


/**
 * @brief Positive test case of haptic_stop_device()
 */
static void utc_system_haptic_stop_device_p(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_vibrate_monotone(0, 1000, 50);
    if(error != HAPTIC_ERROR_NONE)
        dts_fail(API_NAME_STOP_PLAY);

    error = haptic_stop_device(0);
    if(error != HAPTIC_ERROR_NONE)
        dts_fail(API_NAME_STOP_PLAY);

    dts_pass(API_NAME_STOP_PLAY);
}

/**
 * @brief Negative test case of haptic_stop_device()
 */
static void utc_system_haptic_stop_device_n(void)
{
    int error = HAPTIC_ERROR_NONE;

    error = haptic_stop_device(-1);
    if(error == HAPTIC_ERROR_NONE)
        dts_fail(API_NAME_STOP_PLAY);

    dts_pass(API_NAME_STOP_PLAY);
}

static void utc_system_haptic_vibrate_p(void)
{
    haptic_vibration_iter_s ptn[] = {
        {0, HAPTIC_LEVEL_5, 500},
        {0, HAPTIC_LEVEL_1, 500},
    };

    int ptn_id;
    if(haptic_play_pattern(ptn, 2, 1, 0, &ptn_id) < 0){
        dts_fail(API_NAME_PLAY_PTN);
    }else{
        dts_pass(API_NAME_PLAY_PTN);
    }
}

static void utc_system_haptic_vibrate_n(void)
{
    int ptn_id;
    if(haptic_play_pattern(NULL, 7, 1, 0, &ptn_id) < 0){
        dts_pass(API_NAME_PLAY_PTN);
    }else{
        dts_fail(API_NAME_PLAY_PTN);
    }
}

static void utc_system_haptic_stop_pattern_p(void)
{
    haptic_vibration_iter_s ptn[] = {
        {0, HAPTIC_LEVEL_5, 500},
        {0, HAPTIC_LEVEL_1, 500},
    };

    int ptn_id;
    haptic_play_pattern(ptn, 2, 1, 0, &ptn_id);

    int err = haptic_stop_pattern(ptn_id);
    if(err < 0){
        dts_fail(API_NAME_STOP_PTN);
    }
    else{
        dts_pass(API_NAME_STOP_PTN);
    }
}

static void utc_system_haptic_stop_pattern_n(void)
{
    int err = haptic_stop_pattern(-1);
    if(err < 0){
        dts_pass(API_NAME_STOP_PTN);
    }
    else{
        dts_fail(API_NAME_STOP_PTN);
    }

}
