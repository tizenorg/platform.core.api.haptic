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




#include <stdio.h> // xxx
#include <unistd.h>
#include <devman.h>
#include <devman_haptic.h>
#include "haptic.h"

#define IMMERSION_DRV_NODE	"/dev/tspdrv"
#define INPUT_PARM_MAX		0xFFFFFF
#define	COMP_CMD(idx, value)	(value<<4)|(idx&0x0F)

enum {
    HAPTIC_PROP_ENABLE,
    HAPTIC_PROP_LEVEL,
    HAPTIC_PROP_LEVEL_MAX,
    HAPTIC_PROP_VALUE,
    HAPTIC_PROP_ONESHOT,
};

enum {
	CMD_GET_INITIALIZE,
	CMD_SET_OPEN_DEVICE,
	CMD_GET_CURRENT_DEVICE_HANDLE,
	CMD_GET_CURRENT_EFFECT_HANDLE,
	CMD_SET_EFFECT_TYPE,
	CMD_SET_EFFECT_FILE,
	CMD_GET_EFFECT_DURATION,
	CMD_SET_EFFECT_PLAY,
	CMD_SET_EFFECT_PLAY_REPEAT,
	CMD_SET_NORMAL_PLAY,
	CMD_SET_EFFECT_ALL_STOP,
	CMD_PROPERTY_SET_LICENSE_KEY,
	CMD_PROPERTY_GET_PRIORITY,
	CMD_PROPERTY_SET_PRIORITY,
	CMD_PROPERTY_GET_STRENGTH,
	CMD_PROPERTY_SET_STRENGTH,
	CMD_PROPERTY_GET_MASTER_STRENGTH,
	CMD_PROPERTY_SET_MASTER_STRENGTH,
	CMD_SET_CLOSE_DEVICE,
	CMD_GET_TERMINATE,
	CMD_GET_DEVICE_COUNT,
};

static int immersion;

static int IMM_LEVEL[] = {
    0, 0, 2000, 4000, 6000, 8000, 10000
};

static int ETC_LEVEL[] = {
    0,0, 55, 60, 65, 70, 80
};

void _haptic_init()
{
    immersion = access(IMMERSION_DRV_NODE, F_OK) == 0;
}

void _haptic_deinit()
{
}

int _haptic_play_monotone(int dev, int duration, haptic_level_e level)
{
    int status;
    int input = 0;
    unsigned int cmd;

    if(duration < 1 || duration > INPUT_PARM_MAX)
        return HAPTIC_ERROR_INVALID_PARAMETER;

    dev = DEV_IDX_ALL; // xxx

    if(dev != DEV_IDX_ALL && dev != DEV_IDX_0 && dev != DEV_IDX_1)
        return HAPTIC_ERROR_INVALID_PARAMETER;

    if(level == HAPTIC_LEVEL_0)
        return HAPTIC_ERROR_NONE;

    if(level < HAPTIC_LEVEL_1 || level > HAPTIC_LEVEL_5)
        return HAPTIC_ERROR_INVALID_PARAMETER;

    input = immersion ? IMM_LEVEL[level] : ETC_LEVEL[level];
    if(input == 0)
        return HAPTIC_ERROR_NONE;

    if (immersion) {
        cmd = COMP_CMD(dev, input);
        status = device_set_property(DEVTYPE_EFFECT_HAPTIC,
                CMD_PROPERTY_SET_STRENGTH, (int)cmd);
//        printf("capi [%d - %d - %d] [%d %d]\n", DEVTYPE_EFFECT_HAPTIC, CMD_PROPERTY_SET_STRENGTH, cmd, dev, input);
        if (status < 0) {
            return HAPTIC_ERROR_OPERATION_FAILED;
        }

        cmd = COMP_CMD(dev, 0x05);
        status =
            device_set_property(DEVTYPE_EFFECT_HAPTIC,
                    CMD_PROPERTY_SET_PRIORITY, (int)cmd);
        if (status < 0) {
            return HAPTIC_ERROR_OPERATION_FAILED;
        }
    }

    if (immersion) {
        cmd = COMP_CMD(dev, duration);
        status =
            device_set_property(DEVTYPE_EFFECT_HAPTIC,
                    CMD_SET_NORMAL_PLAY, (int)cmd);
        if (status < 0) {
            return HAPTIC_ERROR_OPERATION_FAILED;
        }
    } else {
        status =
            device_set_property(DEVTYPE_HAPTIC, HAPTIC_PROP_LEVEL,
                    input);
        if (status < 0) {
            return HAPTIC_ERROR_OPERATION_FAILED;
        }

        status =
            device_set_property(DEVTYPE_HAPTIC, HAPTIC_PROP_ONESHOT,
                    duration);
        if (status < 0) {
            return HAPTIC_ERROR_OPERATION_FAILED;
        }
    }
    return HAPTIC_ERROR_NONE;
}
