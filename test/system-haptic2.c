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
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>
#include <haptic.h>

#define TEST_IVT "/usr/share/immersion/01_Touch/touch_20ms_sharp.ivt"

static GMainLoop *mainloop;

static void sig_quit(int signo)
{
	if(mainloop)
	{
		g_main_loop_quit(mainloop);
	}
}

static char* error_msg(int err){
    switch(err){
        case HAPTIC_ERROR_NONE                  :
            return "HAPTIC_ERROR_NONE";
        case HAPTIC_ERROR_INVALID_PARAMETER     :
            return "HAPTIC_ERROR_INVALID_PARAMETER";
        case HAPTIC_ERROR_NO_SUCH_FILE          :
            return "HAPTIC_ERROR_NO_SUCH_FILE";
        case HAPTIC_ERROR_NOT_SUPPORTED_FORMAT  :
            return "HAPTIC_ERROR_NOT_SUPPORTED_FORMAT";
        case HAPTIC_ERROR_NOT_INITIALIZED       :
            return "HAPTIC_ERROR_NOT_INITIALIZED";
        case HAPTIC_ERROR_OPERATION_FAILED      :
            return "HAPTIC_ERROR_OPERATION_FAILED";
    }
    return "------??";
}

int main(int argc, char *argv[])
{
    int count, i, err;
	if(haptic_initialize() == HAPTIC_ERROR_NONE){
		printf("haptic device opened\n");
	}else{
		printf("haptic_open fail\n");
	}

	signal(SIGINT, sig_quit);
	signal(SIGTERM, sig_quit);
	signal(SIGQUIT, sig_quit);

	mainloop = g_main_loop_new(NULL, FALSE);

    haptic_get_count(&count);

    printf("count = %d\n", count);

    for(i=0; i<=count;i++){
        printf("play with device(%d)\n", i);

        if((err = haptic_vibrate_monotone(count, 1000)) < 0){
            printf("haptic play fail [%s]\n", error_msg(err));
        }else{
            printf("haptic play success\n");
        }

        if(haptic_stop_device(count) < 0){
            printf("haptic play fail\n");
        }else{
            printf("haptic play success\n");
        }

        if(haptic_vibrate_file(count, TEST_IVT, 1, HAPTIC_LEVEL_AUTO) < 0){
            printf("haptic play fail\n");
        }else{
            printf("haptic play success\n");
        }
    }


	g_main_loop_run(mainloop);
	g_main_loop_unref(mainloop);

	if(haptic_deinitialize() == HAPTIC_ERROR_NONE){
		printf("haptic device closed\n");
	}else{
		printf("haptic_close fail\n");
	}

	return 0;
}
