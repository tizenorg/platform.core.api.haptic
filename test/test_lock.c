#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>
#include <haptic.h>

static GMainLoop *mainloop;
static int ptn_id;

static void sig_quit(int signo)
{
	if(mainloop)
	{
		g_main_loop_quit(mainloop);
	}

	if(haptic_deinitialize() == HAPTIC_ERROR_NONE){
		printf("haptic device closed\n");
	}else{
		printf("haptic_close fail\n");
	}
}

static gboolean play_ptn(gpointer data)
{
    haptic_vibration_iter_s ptn[] = {
        {0, HAPTIC_LEVEL_5, 1000},
        {0, HAPTIC_LEVEL_1, 1000},
        {0, HAPTIC_LEVEL_1, 1000},
        {0, HAPTIC_LEVEL_5, 1000},
        {0, HAPTIC_LEVEL_1, 1000},
        {0, HAPTIC_LEVEL_1, 1000},
        {0, HAPTIC_LEVEL_5, 1000},
    };

    if(haptic_play_pattern(ptn, 7, 1, 0, &ptn_id) < 0){
        printf("haptic play fail\n");
    }else{
        printf("haptic play success\n");
    }

    return false;
}

static gboolean stop_play(gpointer data)
{
    int err;
    int id = (int)data;

    printf("-- stop!! [%d]\n", id);
    err = haptic_stop_pattern(id);
    if(err < 0){
        printf("-- error!! when stop play!!\n");
    }

    return false;
}
static gboolean stop_and_play(gpointer data)
{
    int err;
    int id = (int)data;

    printf("-- stop!! [%d]\n", id);
    err = haptic_stop_pattern(id);
    if(err < 0){
        printf("-- error!! when stop play!!\n");
    }

//    g_timeout_add(6000, play_ptn, NULL);
    g_timeout_add(10000, play_ptn, NULL);

    return false;
}

static gboolean stop_device(gpointer data)
{
    haptic_stop_device(0);

    return false;
}

int main(int argc, char *argv[])
{
    haptic_vibration_iter_s ptn[] = {
        {0, HAPTIC_LEVEL_1, 3000},
        {0, HAPTIC_LEVEL_0, 10},
        {0, HAPTIC_LEVEL_3, 2000},
        {0, HAPTIC_LEVEL_0, 10},
        {0, HAPTIC_LEVEL_5, 1000},
        {0, HAPTIC_LEVEL_0, 10},
        {0, HAPTIC_LEVEL_2, 3000},
    };

	signal(SIGINT, sig_quit);
	signal(SIGTERM, sig_quit);
	signal(SIGQUIT, sig_quit);

	mainloop = g_main_loop_new(NULL, FALSE);

	if(haptic_initialize() == HAPTIC_ERROR_NONE){
		printf("haptic device opened\n");
	}else{
		printf("haptic_open fail\n");
	}
    if(haptic_play_pattern(ptn, 7, 1, 0, &ptn_id) < 0){
        printf("haptic play fail\n");
    }else{
        printf("haptic play success\n");
    }
	if(haptic_deinitialize() == HAPTIC_ERROR_NONE){
		printf("haptic device closed\n");
	}else{
		printf("haptic_close fail\n");
	}
	if(haptic_initialize() == HAPTIC_ERROR_NONE){
		printf("haptic device opened\n");
	}else{
		printf("haptic_open fail\n");
	}
    if(haptic_play_pattern(ptn, 7, 1, 0, &ptn_id) < 0){
        printf("haptic play fail\n");
    }else{
        printf("haptic play success\n");
    }
	if(haptic_deinitialize() == HAPTIC_ERROR_NONE){
		printf("haptic device closed\n");
	}else{
		printf("haptic_close fail\n");
	}


	g_main_loop_run(mainloop);
	g_main_loop_unref(mainloop);

	return 0;
}
