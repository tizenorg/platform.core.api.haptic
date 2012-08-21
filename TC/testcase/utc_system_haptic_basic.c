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

#define API_NAME_HAPTIC_GET_DEVICE_COUNT "haptic_get_count"
#define API_NAME_HAPTIC_INITALIZE "haptic_initialize"
#define API_NAME_HAPTIC_DEINITALIZE "haptic_deinitialize"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_haptic_get_count_p(void);
static void utc_haptic_initialize_p(void);
static void utc_haptic_deinitialize_p(void);
static void utc_haptic_deinitialize_n(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
    { utc_haptic_get_count_p, POSITIVE_TC_IDX},
    { utc_haptic_initialize_p, POSITIVE_TC_IDX},
    { utc_haptic_deinitialize_p, POSITIVE_TC_IDX},
    { utc_haptic_deinitialize_n, NEGATIVE_TC_IDX},
	{ NULL, 0},
};

static void startup(void)
{
	
}

static void cleanup(void)
{
	/* end of TC */
}

static void utc_haptic_get_count_p(void)
{
    int cnt, err;
    err = haptic_get_count(&cnt);

    if(err < 0 ){
        dts_fail(API_NAME_HAPTIC_GET_DEVICE_COUNT);
    }
    else{
        dts_pass(API_NAME_HAPTIC_GET_DEVICE_COUNT);
    }
}

static void utc_haptic_initialize_p(void)
{
    int err = haptic_initialize();
    haptic_deinitialize();

    if(err < 0){
        dts_fail(API_NAME_HAPTIC_INITALIZE);
    }
    else{
        dts_pass(API_NAME_HAPTIC_INITALIZE);
    }
}

static void utc_haptic_deinitialize_p(void)
{
    int err;

    haptic_initialize();
    err = haptic_deinitialize();

    if(err < 0){
        dts_fail(API_NAME_HAPTIC_DEINITALIZE);
    }
    else{
        dts_pass(API_NAME_HAPTIC_DEINITALIZE);
    }
}

static void utc_haptic_deinitialize_n(void)
{
    int err;

    err = haptic_deinitialize();

    if(err < 0){
        dts_pass(API_NAME_HAPTIC_DEINITALIZE);
    }
    else{
        dts_fail(API_NAME_HAPTIC_DEINITALIZE);
    }
}
