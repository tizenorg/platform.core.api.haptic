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

void _haptic_init()
{
    immersion = access(IMMERSION_DRV_NODE, F_OK) == 0;
}

void _haptic_deinit()
{
}

