/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <haptic.h>
#include <devman_haptic.h>
#include <glib.h>
#include <dlog.h>

#include "haptic_private.h"

#undef LOG_TAG
#define LOG_TAG "TIZEN_SYSTEM_HAPTIC"

#define NOT_ASSIGNED -1
#define VIBE_SILENCE -128

#define _MSG_HAPTIC_ERROR_INVALID_PARAMETER "Invalid parameter"
#define _MSG_HAPTIC_ERROR_NO_SUCH_FILE "No such file"
#define _MSG_HAPTIC_ERROR_NOT_SUPPORTED_FORMAT "Not supported format"
#define _MSG_HAPTIC_ERROR_NOT_INITIALIZED "Not initialize"
#define _MSG_HAPTIC_ERROR_OPERATION_FAILED "Operation failed"

#define RETURN_ERR_MSG(err_code, msg) \
    do { \
        LOGE("[%s] "_MSG_##err_code"(0x%08x) : %s", __FUNCTION__, err_code, msg); \
        return err_code; \
    }while(0)

#define RETURN_ERR(err_code) \
    do { \
        LOGE("[%s] "_MSG_##err_code"(0x%08x)", __FUNCTION__, err_code); \
        return err_code; \
    }while(0)

static int _DEV[] = {
    DEV_IDX_ALL,
    DEV_IDX_0,
    DEV_IDX_1,
};

static int _LEVEL[] = 
{
	HAPTIC_FEEDBACK_LEVEL_AUTO,
    VIBE_SILENCE,
	HAPTIC_FEEDBACK_LEVEL_1,
	HAPTIC_FEEDBACK_LEVEL_2,
	HAPTIC_FEEDBACK_LEVEL_3,
	HAPTIC_FEEDBACK_LEVEL_4,
	HAPTIC_FEEDBACK_LEVEL_5,
};

struct _vibe_pattern {
    haptic_vibration_iter_s *iters;
    int current;
    int size;
    int level_change;
    int iter_count;
    int stop;
    int pattern_index;
    int error;
};

GArray *pattern_table = NULL;

static int initialize = 0;
static int max_device = 0;

static int* haptic_ids = NULL;

static void _free_pattern_from_table(int index);

static bool invalid_ivt(const char* file_name)
{
    if( 0 != access(file_name, R_OK) )
    {
        return false;
    }

    char* ext = strrchr(file_name, '.');
    if (ext == NULL || 0 != strcasecmp(ext, ".ivt")){
        return false;
    }

    return true;
}

int haptic_get_count(int* vibrator_number)
{
    int count;
    if(vibrator_number == NULL)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    count = device_haptic_get_device_count();
    if(count < 0)
        RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);

    *vibrator_number = count;

    return HAPTIC_ERROR_NONE;
}

int haptic_initialize()
{
    int i, j; 
    int id;

    if( haptic_get_count(&max_device) < 0)
        RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);

    haptic_ids = (int*)malloc(sizeof(int)*max_device+1); // max + zero

    if(haptic_ids == NULL){
        RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
    }

    for(i=0; i<=max_device; i++){
        id = device_haptic_open(_DEV[i], 0);
        if(id < 0) {
            for (j=i; i>=0; i--){
                device_haptic_close(haptic_ids[i]);
            }
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        }

        haptic_ids[i] = id;
    }

    initialize = 1;

    pattern_table = g_array_new(FALSE, TRUE, sizeof(struct _vibe_pattern*));

    _haptic_init();

    return HAPTIC_ERROR_NONE;
}

int haptic_deinitialize()
{
    int err, i;

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

    for(i=0; i<=max_device; i++){
        err = device_haptic_close(haptic_ids[i]);
    }
    initialize = 0;

    if(haptic_ids != NULL)
        free(haptic_ids);

    for(i=0; i< pattern_table->len; i++){
        _free_pattern_from_table(i);
    }
    g_array_free(pattern_table, TRUE);

    _haptic_deinit();

    return HAPTIC_ERROR_NONE;
}


int haptic_get_file_duration(int device_index, const char *file_name , int* duration)
{
	int ret;

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

    if(device_index < 0 || device_index > max_device)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);
    
    device_index = ((device_index < 3) ? device_index : 0); // xxx

    if(!invalid_ivt(file_name))
        RETURN_ERR(HAPTIC_ERROR_NOT_SUPPORTED_FORMAT);

	ret = device_haptic_get_file_duration(haptic_ids[device_index], file_name, duration);

	if(ret < 0){
        if(ret == -2)
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        else
            RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);
    }
	
	return HAPTIC_ERROR_NONE;
}

int haptic_vibrate_file(int device_index, const char *file_name , int count , haptic_level_e level)
{
	int ret;

    if(device_index < 0 || device_index > max_device)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    device_index = ((device_index < 3) ? device_index : 0); // xxx

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

    if(level == VIBE_SILENCE)
        return HAPTIC_ERROR_NONE;

	if(level > HAPTIC_LEVEL_5 || level < 0)
		RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    if(count < 0 || count >= HAPTIC_INFINITE_ITERATION)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    if(count == 0)
        count = HAPTIC_INFINITE_ITERATION;

    if(!invalid_ivt(file_name))
        RETURN_ERR(HAPTIC_ERROR_NOT_SUPPORTED_FORMAT);

	ret = device_haptic_play_file(haptic_ids[device_index], file_name, count, _LEVEL[level]);

	if(ret < 0){
        if(ret == -2)
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        else
            RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);
    }
	
	return HAPTIC_ERROR_NONE;
}

int haptic_vibrate_monotone(int device_index , int duration)
{
	int ret;

    if(device_index < 0 || device_index > max_device)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    device_index = ((device_index < 3) ? device_index : 0); // xxx

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

	ret = device_haptic_play_monotone(haptic_ids[device_index], duration);

	if(ret < 0){
        if(ret == -2)
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        else
            RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);
    }
	
	return HAPTIC_ERROR_NONE;
}

int haptic_stop_device(int device_index)
{
	int ret;

    if(device_index < 0 || device_index > max_device)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    device_index = ((device_index < 3) ? device_index : 0); // xxx

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

	ret = device_haptic_stop_play(haptic_ids[device_index]);

	if(ret < 0){
        if(ret == -2)
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        else
            RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);
    }
	
	return HAPTIC_ERROR_NONE;
}

static void _free_pattern_from_table(int index)
{
    struct _vibe_pattern* p = g_array_index(pattern_table, struct _vibe_pattern *, index);
    if(p == NULL)
        return;
    free(p->iters);
    free(p);
    g_array_index(pattern_table, struct _vibe_pattern *, index) = NULL;
}

static int _haptic_play_monotone(int device_index, long duration, haptic_level_e level)
{
	int ret;

    if(device_index < 0 || device_index > max_device)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    device_index = ((device_index < 3) ? device_index : 0); // xxx

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);


	ret = device_haptic_play_monotone_with_feedback_level(haptic_ids[device_index], duration, _LEVEL[level]);

	if(ret < 0){
        if(ret == -2)
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        else
            RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);
    }

	return HAPTIC_ERROR_NONE;
}

static gboolean _haptic_play_iter(gpointer data)
{
    int err;
    struct _vibe_pattern* pattern = (struct _vibe_pattern*)data;

    if(pattern->stop){
        _free_pattern_from_table(pattern->pattern_index);
        return false;
    }
    int current = pattern->current;

    int device = pattern->iters[current].vibrator_index;
    int level = pattern->iters[current].level;
    long time = pattern->iters[current].time;
    int level_change = pattern->level_change;
    int iter_count = pattern->iter_count;
    
    // set default device, if can't find given device.
    if(device >= max_device || device < 0)
        device = 0;

    if(level + level_change < HAPTIC_LEVEL_0)
        level = HAPTIC_LEVEL_0;
    if(level + level_change > HAPTIC_LEVEL_5)
        level = HAPTIC_LEVEL_5;

    if(level != HAPTIC_LEVEL_0 || time != 0){
        err = _haptic_play_monotone(device, time, level);
        if(err<0){
            pattern->error = err;
            return false;
        }
    }

    pattern->current++;

    // pattern play finish
    if(pattern->current >= pattern->size){
        if(iter_count <= 0){
            _free_pattern_from_table(pattern->pattern_index);
            return false; 
        }else{
            pattern->current = 0;
            pattern->iter_count--;
        }
    }


    g_timeout_add(time, _haptic_play_iter, data);

    return false;
}

int haptic_play_pattern(haptic_vibration_iter_s* pattern, int pattern_size, int count, int level_change, int* id)
{
    int i, key = -1;

    if(id == NULL)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    if(pattern == NULL)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    if(!initialize)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

    haptic_vibration_iter_s* tmp_ptn = (haptic_vibration_iter_s*)
        malloc(sizeof(haptic_vibration_iter_s) * pattern_size);
    if(tmp_ptn == NULL){
        RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
    }
    memcpy(tmp_ptn, pattern, sizeof(haptic_vibration_iter_s) * pattern_size);

    struct _vibe_pattern* vibe_p = (struct _vibe_pattern*)malloc(sizeof(struct _vibe_pattern));
    if(vibe_p == NULL){
        free(tmp_ptn);
        RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
    }
    vibe_p->iters = tmp_ptn;
    vibe_p->size = pattern_size;
    vibe_p->level_change = level_change;
    vibe_p->iter_count = count;
    vibe_p->current = 0;
    vibe_p->stop = 0;
    vibe_p->error= 0;

    for(i=0; i< pattern_table->len; i++){
        if(g_array_index(pattern_table, struct _vibe_pattern *, i) == NULL){
            key = i;
            break;
        }
    }
    if(key == -1){
        g_array_append_val(pattern_table, vibe_p);
        key = pattern_table->len -1;
    }else{
        g_array_index(pattern_table, struct _vibe_pattern *, key) = vibe_p;
    }

    vibe_p->pattern_index = key;

    _haptic_play_iter((gpointer)vibe_p);

    if(vibe_p->error < 0){
        _free_pattern_from_table(key);
		if(vibe_p->error == -2)
            RETURN_ERR(HAPTIC_ERROR_OPERATION_FAILED);
        else
            RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);
    }

    *id = key;

    return HAPTIC_ERROR_NONE;
}

int haptic_stop_pattern(int id)
{
    if(id < 0)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    if(!initialize || pattern_table == NULL)
        RETURN_ERR(HAPTIC_ERROR_NOT_INITIALIZED);

    if(id >= pattern_table->len)
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    struct _vibe_pattern* pattern = g_array_index(pattern_table, struct _vibe_pattern *, id);
    if(pattern != NULL)
        pattern->stop = 1;
    else
        RETURN_ERR(HAPTIC_ERROR_INVALID_PARAMETER);

    return HAPTIC_ERROR_NONE;
}
