
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <haptic.h>

#include <Elementary.h>
#include <Ecore_X.h>

static haptic_vibration_iter_s ptn[] = {
    {0, HAPTIC_LEVEL_5, 1000},
    {0, HAPTIC_LEVEL_1, 1000},
    {0, HAPTIC_LEVEL_1, 1000},
    {0, HAPTIC_LEVEL_5, 1000},
    {0, HAPTIC_LEVEL_1, 1000},
    {0, HAPTIC_LEVEL_1, 1000},
    {0, HAPTIC_LEVEL_5, 1000},
};

int ptn_id;

static void _quit_cb(void *data, Evas_Object* obj, void* event_info)
{
	Evas_Object *win = (Evas_Object *) data;
	elm_win_lower(win);
}

static void _vibe_clicked_cb(void *data, Evas_Object* obj, void* event_info)
{
    printf("initialize!\n");
    haptic_initialize();
    printf("after initialize!\n");

    printf("play pattern!\n");
    haptic_play_pattern(ptn, 7, 1, 0, &ptn_id);
    printf("after play pattern!\n");
}

static void _stop_clicked_cb(void *data, Evas_Object* obj, void* event_info)
{
    int err;
    printf("stop pattern!\n");
    err = haptic_stop_pattern(ptn_id);
    printf("after stop pattern! [%d]\n", err);

    printf("deinitialize!\n");
    haptic_deinitialize();
    printf("after deinitialize!\n");
}

static void winmain()
{
    const char* name = "Hapti_UI_TEST";
    int w,h;

    Evas_Object* win;
    Evas_Object* bg;
    Evas_Object* main_layout;
    Evas_Object* btns_box;
    Evas_Object* btn_vibe;
    Evas_Object* btn_stop;

    Evas* evas;

    win = elm_win_add(NULL, name, ELM_WIN_BASIC);
    if(!win)
        return;

    elm_win_title_set(win, name);
	elm_win_borderless_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);
	evas_object_smart_callback_add(win, "delete,request", _quit_cb, NULL);
	ecore_x_window_size_get(ecore_x_window_root_first_get(), &w, &h);
	evas_object_resize(win, w, h);

    evas_object_show(win);

    evas = evas_object_evas_get(win);

    // set background
    bg = elm_bg_add(win);
    elm_bg_color_set(bg, 0xff, 0xff, 0xff); 
    evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, bg);
    evas_object_show(bg);

    // Base Layout
    main_layout = elm_layout_add(win);
    elm_layout_theme_set(main_layout, "layout", "application", "default");
    evas_object_size_hint_weight_set(main_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, main_layout);
    evas_object_show(main_layout);

    // Indicator
    elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);


    // add box
    btns_box = elm_box_add(win);
    elm_box_horizontal_set(btns_box, EINA_TRUE);
    elm_win_resize_object_add(win, btns_box);
    elm_box_padding_set(btns_box, 10, 0);
    evas_object_show(btns_box);

    // add Button
    btn_vibe = elm_button_add(main_layout);
    btn_stop = elm_button_add(main_layout);
    elm_object_text_set(btn_vibe, "Vibe");
    elm_object_text_set(btn_stop, "Stop");
    evas_object_smart_callback_add(btn_vibe, "clicked", _vibe_clicked_cb, NULL);
    evas_object_smart_callback_add(btn_stop, "clicked", _stop_clicked_cb, NULL);

    elm_box_pack_end(btns_box, btn_vibe);
    elm_box_pack_end(btns_box, btn_stop);
    evas_object_show(btn_vibe);
    evas_object_show(btn_stop);
}

EAPI_MAIN int elm_main(int argc, char** argv)
{
    winmain();

    elm_run();
    elm_shutdown();
    return 0;
}
ELM_MAIN()
