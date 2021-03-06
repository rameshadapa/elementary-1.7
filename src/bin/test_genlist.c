#include <Elementary_Cursor.h>
#include "test.h"
#ifdef HAVE_CONFIG_H
#include "elementary_config.h"
#endif
#include <Elementary.h>
#ifndef ELM_LIB_QUICKLAUNCH
struct _api_data
{
   unsigned int state;  /* What state we are testing       */
   void *box;           /* Use this to get box content     */
};
typedef struct _api_data api_data;

enum _api_state
{
   HORIZONTAL_SET,
   NO_SELECT_MODE_SET,
   COMPRESS_MODE_SET,
   BOUNCE_SET,
   HOMOGENEOUS_SET,
   SCROLLER_POLICY_SET,
   TOOLTIP_TEXT_SET,
   ITEM_CURSOR_SET,
   API_STATE_LAST
};
typedef enum _api_state api_state;

static void
set_api_state(api_data *api)
{
   const Eina_List *items = elm_box_children_get(api->box);
   if (!eina_list_count(items))
     return;

   Evas_Object *gl = eina_list_nth(items, 0); /* Genlist is the first item */
   /* use elm_box_children_get() to get list of children */
   switch(api->state)
     {/* Put all api-changes under switch */
      case HORIZONTAL_SET: /* 0 */
         elm_genlist_mode_set(gl, ELM_LIST_LIMIT);
         break;

      case NO_SELECT_MODE_SET: /* 1 */
         elm_genlist_select_mode_set(gl, ELM_OBJECT_SELECT_MODE_NONE);
         elm_genlist_mode_set(gl, ELM_LIST_SCROLL); /* Back to default */
         break;

      case COMPRESS_MODE_SET: /* 2 */
         elm_genlist_select_mode_set(gl, ELM_OBJECT_SELECT_MODE_DEFAULT);
         elm_genlist_mode_set(gl, ELM_LIST_COMPRESS);
         break;

      case BOUNCE_SET: /* 3 */
         elm_scroller_bounce_set(gl, EINA_FALSE, EINA_FALSE);
         break;

      case HOMOGENEOUS_SET: /* 4 */
         elm_scroller_bounce_set(gl, EINA_TRUE, EINA_TRUE); /* Back to default */
         elm_genlist_homogeneous_set(gl, EINA_TRUE);
         break;

      case SCROLLER_POLICY_SET: /* 5 */
         elm_genlist_homogeneous_set(gl, EINA_FALSE); /* Back to default */
         elm_scroller_policy_set(gl, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
         break;

      case TOOLTIP_TEXT_SET: /* 6 */
           {
              elm_genlist_item_tooltip_text_set(elm_genlist_first_item_get(gl), "Tooltip text from API");
              elm_genlist_item_bring_in(elm_genlist_first_item_get(gl),
                                         ELM_GENLIST_ITEM_SCROLLTO_IN);
           }
         break;

      case ITEM_CURSOR_SET: /* 7 */
         elm_genlist_item_cursor_set(elm_genlist_first_item_get(gl), ELM_CURSOR_HAND2);
         break;

      default:
         return;
     }
}

static void
_api_bt_clicked(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{  /* Will add here a SWITCH command containing code to modify test-object */
   /* in accordance a->state value. */
   api_data *a = data;
   char str[128];

   INF("clicked event on API Button: api_state=<%d>", a->state);
   set_api_state(a);
   a->state++;
   sprintf(str, "Next API function (%u)", a->state);
   elm_object_text_set(obj, str);
   elm_object_disabled_set(obj, a->state == API_STATE_LAST);
}
/* Hint:
 * In this example some calls to elm_genlist_item_append() are used which give the
 * value of an int as 'item data' and 'func data' after casting into (void*). For
 * sure this works only on architectures where sizeof(int)==sizeof(void*).
 * For real world usage you should hold a data structure or value in your
 * application and then give only a pointer to this data as data pointer.
 */

typedef struct _Testitem
{
   Elm_Object_Item *item;
   int mode;
   int onoff;
   Eina_Bool checked;
} Testitem;

static Elm_Genlist_Item_Class *itc1;
char *gl_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "Item # %i", (int)(long)data);
   return strdup(buf);
}

Evas_Object *gl_content_get(void *data __UNUSED__, Evas_Object *obj, const char *part)
{
   char buf[PATH_MAX];
   Evas_Object *ic = elm_icon_add(obj);
   if (!strcmp(part, "elm.swallow.end"))
     snprintf(buf, sizeof(buf), "%s/images/bubble.png", elm_app_data_dir_get());
   else
     snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
   elm_image_file_set(ic, buf, NULL);
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void on_bt_clicked(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   printf("button clicked\n");
}

static void on_ck_changed(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   printf("checkbox changed\n");
}

Evas_Object *gl_content_full_get(void *data __UNUSED__, Evas_Object *obj, const char *part)
{
   Evas_Object *fr, *bx, *ic, *bt, *ck;

   if (strcmp(part, "elm.swallow.content") != 0) return NULL;

   fr = elm_frame_add(obj);
   elm_layout_text_set(fr, NULL, "A Frame");

   bx = elm_box_add(fr);
   elm_box_horizontal_set(bx, EINA_TRUE);

   ic = elm_icon_add(bx);
   elm_icon_standard_set(ic, "home");
   evas_object_size_hint_min_set(ic, 32, 32);
   evas_object_size_hint_align_set(ic, 0.5, EVAS_HINT_FILL);
   evas_object_show(ic);
   elm_box_pack_end(bx, ic);

   bt = elm_button_add(bx);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(bt, "clicked", on_bt_clicked, NULL);
   elm_layout_text_set(bt, NULL, "Click Me");
   evas_object_show(bt);
   elm_box_pack_end(bx, bt);

   ck = elm_check_add(bx);
   evas_object_size_hint_align_set(ck, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(ck, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(ck, "changed", on_ck_changed, NULL);
   elm_layout_text_set(ck, NULL, "some checkbox");
   evas_object_show(ck);
   elm_box_pack_end(bx, ck);

   elm_layout_content_set(fr, NULL, bx);
   evas_object_size_hint_min_set(fr, 100, 100);

   return fr;
}


Eina_Bool gl_state_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return EINA_FALSE;
}

static void
gl_sel(void *data, Evas_Object *obj, void *event_info)
{
   printf("sel item data [%p] on genlist obj [%p], item pointer [%p]\n", data, obj, event_info);
}

static void
_move(void *data, Evas *evas __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Evas_Object *gl = data;
   Evas_Event_Mouse_Move *ev = event_info;
   int where = 0;
   Elm_Object_Item *gli;
   gli = elm_genlist_at_xy_item_get(gl, ev->cur.canvas.x, ev->cur.canvas.y, &where);
   if (gli)
     INF("over %p, where %i", elm_object_item_data_get(gli), where);
   else
     INF("over none, where %i", where);
}

static void
_bt50_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_item_bring_in(data, ELM_GENLIST_ITEM_SCROLLTO_IN);
}

static void
_bt1500_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_item_bring_in(data, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
}

static void
_gl_selected(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   printf("selected: %p\n", event_info);
}

static void
_gl_double_clicked(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   printf("double clicked: %p\n", event_info);
}

static void
_gl_longpress(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   printf("longpress %p\n", event_info);
}

static void
_cleanup_cb(void *data, Evas *e __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   free(data);
}

void
test_genlist(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bt_50, *bt_1500, *bx, *bxx, *bt;
   Evas_Object *over;
   Elm_Object_Item *gli;
   int i;
   api_data *api = calloc(1, sizeof(api_data));

   win = elm_win_util_standard_add("genlist", "Genlist");
   elm_win_autodel_set(win, EINA_TRUE);
   evas_object_event_callback_add(win, EVAS_CALLBACK_FREE, _cleanup_cb, api);

   bxx = elm_box_add(win);
   elm_win_resize_object_add(win, bxx);
   evas_object_size_hint_weight_set(bxx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bxx);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   api->box = bx;
   evas_object_show(bx);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Next API function");
   evas_object_smart_callback_add(bt, "clicked", _api_bt_clicked, (void *) api);
   elm_box_pack_end(bxx, bt);
   elm_object_disabled_set(bt, api->state == API_STATE_LAST);
   evas_object_show(bt);

   elm_box_pack_end(bxx, bx);

   gl = elm_genlist_add(win);
   evas_object_smart_callback_add(gl, "selected", _gl_selected, NULL);
   evas_object_smart_callback_add(gl, "clicked,double", _gl_double_clicked, NULL);
   evas_object_smart_callback_add(gl, "longpressed", _gl_longpress, NULL);
   // FIXME: This causes genlist to resize the horiz axis very slowly :(
   // Reenable this and resize the window horizontally, then try to resize it back
   //elm_genlist_mode_set(gl, ELM_LIST_LIMIT);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, gl);
   evas_object_show(gl);

   over = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_color_set(over, 0, 0, 0, 0);
   evas_object_event_callback_add(over, EVAS_CALLBACK_MOUSE_MOVE, _move, gl);
   evas_object_repeat_events_set(over, 1);
   evas_object_show(over);
   evas_object_size_hint_weight_set(over, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, over);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   bt_50 = elm_button_add(win);
   elm_object_text_set(bt_50, "Go to 50");
   evas_object_show(bt_50);
   elm_box_pack_end(bx, bt_50);

   bt_1500 = elm_button_add(win);
   elm_object_text_set(bt_1500, "Go to 1500");
   evas_object_show(bt_1500);
   elm_box_pack_end(bx, bt_1500);

   for (i = 0; i < 2000; i++)
     {
        gli = elm_genlist_item_append(gl, itc1,
                                      (void *)(long)i/* item data */,
                                      NULL/* parent */,
                                      ELM_GENLIST_ITEM_NONE,
                                      gl_sel/* func */,
                                      (void *)(long)(i * 10)/* func data */);
        if (i == 50)
          evas_object_smart_callback_add(bt_50, "clicked", _bt50_cb, gli);
        else if (i == 1500)
          evas_object_smart_callback_add(bt_1500, "clicked", _bt1500_cb, gli);
     }
   elm_genlist_item_class_free(itc1);

   evas_object_resize(win, 480, 800);
   evas_object_show(win);
}

/*************/

static void
my_gl_clear(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   elm_genlist_clear(gl);
}

static void
my_gl_add(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   static int i = 0;

   if (!itc1) itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   elm_genlist_item_append(gl, itc1,
                           (void *)(long)i/* item data */,
                           NULL/* parent */,
                           ELM_GENLIST_ITEM_NONE,
                           gl_sel/* func */,
                           (void *)(long)(i * 10)/* func data */);
   i++;
}

static void
my_gl_insert_before(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   static int i = 0;
   Elm_Object_Item *gli_selected;

   if (!itc1) itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   gli_selected = elm_genlist_selected_item_get(gl);
   if (!gli_selected)
     {
        printf("no item selected\n");
        return ;
     }

   elm_genlist_item_insert_before(gl, itc1,
                                  (void *)(long)i/* item data */,
                                  NULL/* parent */,
                                  gli_selected /* item before */,
                                  ELM_GENLIST_ITEM_NONE,
                                  gl_sel/* func */,
                                  (void *)(long)(i * 10)/* func data */);
   i++;
}

static void
my_gl_insert_after(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   static int i = 0;
   Elm_Object_Item *gli_selected;

   if (!itc1) itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   gli_selected = elm_genlist_selected_item_get(gl);
   if (!gli_selected)
     {
        printf("no item selected\n");
        return ;
     }

   elm_genlist_item_insert_after(gl, itc1,
                                 (void *)(long)i/* item data */,
                                 NULL/* parent */,
                                 gli_selected /* item after */,
                                 ELM_GENLIST_ITEM_NONE,
                                 gl_sel/* func */,
                                 (void *)(long)(i * 10)/* func data */);
   i++;
}

static void
my_gl_del(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   Elm_Object_Item *gli = elm_genlist_selected_item_get(gl);
   if (!gli)
     {
        printf("no item selected\n");
        return;
     }
   elm_object_item_del(gli);
}

static void
my_gl_disable(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   Elm_Object_Item *gli = elm_genlist_selected_item_get(gl);
   if (!gli)
     {
        printf("no item selected\n");
        return;
     }
   elm_object_item_disabled_set(gli, EINA_TRUE);
   elm_genlist_item_selected_set(gli, EINA_FALSE);
   elm_genlist_item_update(gli);
}

static void
my_gl_update_all(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   int i = 0;
   Elm_Object_Item *it = elm_genlist_first_item_get(gl);
   while (it)
     {
        elm_genlist_item_update(it);
        INF("%i", i);
        i++;
        it = elm_genlist_item_next_get(it);
     }
}

static void
my_gl_first(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   Elm_Object_Item *gli = elm_genlist_first_item_get(gl);
   if (!gli) return;
   elm_genlist_item_show(gli, ELM_GENLIST_ITEM_SCROLLTO_IN);
   elm_genlist_item_selected_set(gli, 1);
}

static void
my_gl_last(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   Elm_Object_Item *gli = elm_genlist_last_item_get(gl);
   if (!gli) return;
   elm_genlist_item_show(gli, ELM_GENLIST_ITEM_SCROLLTO_IN);
   elm_genlist_item_selected_set(gli, 1);
}

static Eina_Bool
my_gl_flush_delay(void *data __UNUSED__)
{
   elm_cache_all_flush();
   return ECORE_CALLBACK_CANCEL;
}

static void
my_gl_flush(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   ecore_timer_add(1.2, my_gl_flush_delay, NULL);
}

void
test_genlist2(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *bg, *gl, *bx, *bx2, *bx3, *bt;
   Elm_Object_Item *gli[10];
   char buf[PATH_MAX];

   win = elm_win_add(NULL, "genlist2", ELM_WIN_BASIC);
   elm_win_title_set(win, "Genlist 2");
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   snprintf(buf, sizeof(buf), "%s/images/plant_01.jpg", elm_app_data_dir_get());
   elm_bg_file_set(bg, buf, NULL);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   elm_genlist_mode_set(gl, ELM_LIST_COMPRESS);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   gli[0] = elm_genlist_item_append(gl, itc1,
                                    (void *)1001/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                    (void *)1001/* func data */);
   gli[1] = elm_genlist_item_append(gl, itc1,
                                    (void *)1002/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                    (void *)1002/* func data */);
   gli[2] = elm_genlist_item_append(gl, itc1,
                                    (void *)1003/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                    (void *)1003/* func data */);
   gli[3] = elm_genlist_item_prepend(gl, itc1,
                                     (void *)1004/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                     (void *)1004/* func data */);
   gli[4] = elm_genlist_item_prepend(gl, itc1,
                                     (void *)1005/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                     (void *)1005/* func data */);
   gli[5] = elm_genlist_item_insert_before(gl, itc1,
                                           (void *)1006/* item data */, NULL/* parent */, gli[2]/* rel */, ELM_GENLIST_ITEM_NONE,
                                           gl_sel/* func */, (void *)1006/* func data */);
   gli[6] = elm_genlist_item_insert_after(gl, itc1,
                                          (void *)1007/* item data */, NULL/* parent */, gli[2]/* rel */, ELM_GENLIST_ITEM_NONE,
                                          gl_sel/* func */, (void *)1007/* func data */);
   elm_box_pack_end(bx, gl);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "/\\");
   evas_object_smart_callback_add(bt, "clicked", my_gl_first, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "\\/");
   evas_object_smart_callback_add(bt, "clicked", my_gl_last, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "#");
   evas_object_smart_callback_add(bt, "clicked", my_gl_disable, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "U");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update_all, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "X");
   evas_object_smart_callback_add(bt, "clicked", my_gl_clear, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "+");
   evas_object_smart_callback_add(bt, "clicked", my_gl_add, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "-");
   evas_object_smart_callback_add(bt, "clicked", my_gl_del, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   bx3 = elm_box_add(win);
   elm_box_horizontal_set(bx3, EINA_TRUE);
   elm_box_homogeneous_set(bx3, EINA_TRUE);
   evas_object_size_hint_weight_set(bx3, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx3, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "+ before");
   evas_object_smart_callback_add(bt, "clicked", my_gl_insert_before, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx3, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "+ after");
   evas_object_smart_callback_add(bt, "clicked", my_gl_insert_after, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx3, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Flush");
   evas_object_smart_callback_add(bt, "clicked", my_gl_flush, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx3, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx3);
   evas_object_show(bx3);

   /* item_class_ref is needed for itc1. some items can be added in callbacks */
   elm_genlist_item_class_ref(itc1);
   elm_genlist_item_class_free(itc1);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static Elm_Genlist_Item_Class itc2;

char *gl2_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   const Testitem *tit = data;
   char buf[256];
   snprintf(buf, sizeof(buf), "Item mode %i", tit->mode);
   return strdup(buf);
}

Evas_Object *gl2_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Testitem *tit = data;
   char buf[PATH_MAX];
   Evas_Object *ic = elm_icon_add(obj);
   if (!strcmp(part, "elm.swallow.icon"))
     {
        if (!(tit->mode & 0x3))
          snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
        else if ((tit->mode & 0x3) == 1)
          snprintf(buf, sizeof(buf), "%s/images/logo.png", elm_app_data_dir_get());
        else if ((tit->mode & 0x3) == 2)
          snprintf(buf, sizeof(buf), "%s/images/panel_01.jpg", elm_app_data_dir_get());
        else if ((tit->mode & 0x3) == 3)
          snprintf(buf, sizeof(buf), "%s/images/rock_01.jpg", elm_app_data_dir_get());
        elm_image_file_set(ic, buf, NULL);
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        if (!(tit->mode & 0x3))
          snprintf(buf, sizeof(buf), "%s/images/sky_01.jpg", elm_app_data_dir_get());
        else if ((tit->mode & 0x3) == 1)
          snprintf(buf, sizeof(buf), "%s/images/sky_02.jpg", elm_app_data_dir_get());
        else if ((tit->mode & 0x3) == 2)
          snprintf(buf, sizeof(buf), "%s/images/sky_03.jpg", elm_app_data_dir_get());
        else if ((tit->mode & 0x3) == 3)
          snprintf(buf, sizeof(buf), "%s/images/sky_04.jpg", elm_app_data_dir_get());
        elm_image_file_set(ic, buf, NULL);
     }
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

Eina_Bool gl2_state_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return EINA_FALSE;
}

static void
my_gl_update(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Testitem *tit = data;
   tit->mode++;
   elm_genlist_item_update(tit->item);
}

static void
my_gl_labels_update(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Testitem *tit = data;
   tit->mode++;
   elm_genlist_item_fields_update(tit->item, "*", ELM_GENLIST_ITEM_FIELD_TEXT);
}

static void
my_gl_contents_update(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Testitem *tit = data;
   tit->mode++;
   elm_genlist_item_fields_update(tit->item, "*", ELM_GENLIST_ITEM_FIELD_CONTENT);
}

void
test_genlist3(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bx, *bx2, *bt;
   static Testitem tit[3];

   win = elm_win_util_standard_add("genlist3", "Genlist 3");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   itc2.item_style     = "default";
   itc2.func.text_get = gl2_text_get;
   itc2.func.content_get  = gl2_content_get;
   itc2.func.state_get = gl2_state_get;
   itc2.func.del       = NULL;

   tit[0].mode = 0;
   tit[0].item = elm_genlist_item_append(gl, &itc2,
                                         &(tit[0])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);
   tit[1].mode = 1;
   tit[1].item = elm_genlist_item_append(gl, &itc2,
                                         &(tit[1])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);
   tit[2].mode = 2;
   tit[2].item = elm_genlist_item_append(gl, &itc2,
                                         &(tit[2])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);

   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[1]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[0]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "labels");
   evas_object_smart_callback_add(bt, "clicked", my_gl_labels_update, &(tit[1]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "contents");
   evas_object_smart_callback_add(bt, "clicked", my_gl_contents_update, &(tit[2]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static void
my_gl_item_check_changed(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Testitem *tit = data;
   tit->onoff = elm_check_state_get(obj);
   printf("item %p onoff = %i\n", tit, tit->onoff);
}

static Elm_Genlist_Item_Class itc3;

char *gl3_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   const Testitem *tit = data;
   char buf[256];
   snprintf(buf, sizeof(buf), "Item mode %i", tit->mode);
   return strdup(buf);
}

Evas_Object *gl3_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Testitem *tit = data;
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *lb;

        lb = elm_label_add(obj);
        elm_label_line_wrap_set(lb, ELM_WRAP_CHAR);
        elm_label_wrap_width_set(lb, 201);
        elm_object_text_set(lb, "ashj ascjscjh n asjkl hcjlh ls hzshnn zjh sh zxjcjsnd h dfw sdv edev efe fwefvv vsd cvs ws wf  fvwf wd fwe f  we wef we wfe rfwewef wfv wswf wefg sdfws w wsdcfwcf wsc vdv  sdsd sdcd cv wsc sdcv wsc d sdcdcsd sdcdsc wdvd sdcsd wscxcv wssvd sd");
        return lb;
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        Evas_Object *ck;
        ck = elm_check_add(obj);
        evas_object_propagate_events_set(ck, EINA_FALSE);
        elm_check_state_set(ck, tit->onoff);
        evas_object_smart_callback_add(ck, "changed", my_gl_item_check_changed, data);
        return ck;
     }
   return NULL;
}

Eina_Bool gl3_state_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return EINA_FALSE;
}

void
test_genlist4(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bx, *bx2, *bt;
   static Testitem tit[3];

   win = elm_win_util_standard_add("genlist4", "Genlist 4");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   elm_genlist_mode_set(gl, ELM_LIST_COMPRESS);
   elm_genlist_multi_select_set(gl, 1);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   itc3.item_style     = "default";
   itc3.func.text_get = gl3_text_get;
   itc3.func.content_get  = gl3_content_get;
   itc3.func.state_get = gl3_state_get;
   itc3.func.del       = NULL;

   tit[0].mode = 0;
   tit[0].item = elm_genlist_item_append(gl, &itc3,
                                         &(tit[0])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);
   tit[1].mode = 1;
   tit[1].item = elm_genlist_item_append(gl, &itc3,
                                         &(tit[1])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);
   tit[2].mode = 2;
   tit[2].item = elm_genlist_item_append(gl, &itc3,
                                         &(tit[2])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);

   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[1]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[0]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[2]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[1]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[3]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[2]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static void
my_gl_item_check_changed2(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Testitem *tit = data;
   tit->onoff = elm_check_state_get(obj);
   printf("item %p onoff = %i\n", tit, tit->onoff);
}

static Elm_Genlist_Item_Class itc5;

char *gl5_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part)
{
   const Testitem *tit = data;
   char buf[256];
   if (!strcmp(part, "elm.text"))
     {
        snprintf(buf, sizeof(buf), "Item mode %i", tit->mode);
     }
   else if (!strcmp(part, "elm.text.sub"))
     {
        snprintf(buf, sizeof(buf), "%i bottles on the wall", tit->mode);
     }
   return strdup(buf);
}

Evas_Object *gl5_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Testitem *tit = data;
   char buf[PATH_MAX];
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *bx = elm_box_add(obj);
        Evas_Object *ic;
        elm_box_horizontal_set(bx, EINA_TRUE);
        ic = elm_icon_add(obj);
        snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
        elm_image_file_set(ic, buf, NULL);
        elm_image_resizable_set(ic, 0, 0);
        evas_object_show(ic);
        elm_box_pack_end(bx, ic);
        ic = elm_icon_add(obj);
        elm_image_file_set(ic, buf, NULL);
        elm_image_resizable_set(ic, 0, 0);
        evas_object_show(ic);
        elm_box_pack_end(bx, ic);
        elm_box_horizontal_set(bx, EINA_TRUE);
        evas_object_show(bx);
        return bx;
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        Evas_Object *ck;
        ck = elm_check_add(obj);
        evas_object_propagate_events_set(ck, EINA_FALSE);
        elm_check_state_set(ck, tit->onoff);
        evas_object_smart_callback_add(ck, "changed", my_gl_item_check_changed2, data);
        evas_object_show(ck);
        return ck;
     }
   return NULL;
}

Eina_Bool gl5_state_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return EINA_FALSE;
}

static void
item_drag_up(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("drag up");
}

static void
item_drag_down(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("drag down");
}

static void
item_drag_left(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("drag left");
}

static void
item_drag_right(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("drag right");
}

static void
scroll_top(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("Top edge!");
}

static void
scroll_bottom(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("Bottom edge!");
}

static void
scroll_left(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("Left edge!");
}

static void
scroll_right(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("Right edge!");
}

static void
item_drag(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("drag");
}

static void
item_drag_stop(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("drag stop");
}

static void
item_longpress(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   INF("longpress");
}

void
test_genlist5(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bx, *bx2, *bt;
   static Testitem tit[3];

   win = elm_win_util_standard_add("genlist5", "Genlist 5");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   elm_genlist_select_mode_set(gl, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);
   itc5.item_style     = "double_label";
   itc5.func.text_get = gl5_text_get;
   itc5.func.content_get  = gl5_content_get;
   itc5.func.state_get = gl5_state_get;
   itc5.func.del       = NULL;

   tit[0].mode = 0;
   tit[0].item = elm_genlist_item_append(gl, &itc5,
                                         &(tit[0])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);
   tit[1].mode = 1;
   tit[1].item = elm_genlist_item_append(gl, &itc5,
                                         &(tit[1])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);
   tit[2].mode = 2;
   tit[2].item = elm_genlist_item_append(gl, &itc5,
                                         &(tit[2])/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl_sel/* func */,
                                         NULL/* func data */);

   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   evas_object_smart_callback_add(gl, "drag,start,up", item_drag_up, NULL);
   evas_object_smart_callback_add(gl, "drag,start,down", item_drag_down, NULL);
   evas_object_smart_callback_add(gl, "drag,start,left", item_drag_left, NULL);
   evas_object_smart_callback_add(gl, "drag,start,right", item_drag_right, NULL);
   evas_object_smart_callback_add(gl, "scroll,edge,top", scroll_top, NULL);
   evas_object_smart_callback_add(gl, "scroll,edge,bottom", scroll_bottom, NULL);
   evas_object_smart_callback_add(gl, "scroll,edge,left", scroll_left, NULL);
   evas_object_smart_callback_add(gl, "scroll,edge,right", scroll_right, NULL);
   evas_object_smart_callback_add(gl, "drag", item_drag, NULL);
   evas_object_smart_callback_add(gl, "drag,stop", item_drag_stop, NULL);
   evas_object_smart_callback_add(gl, "longpressed", item_longpress, NULL);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[1]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[0]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[2]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[1]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "[3]");
   evas_object_smart_callback_add(bt, "clicked", my_gl_update, &(tit[2]));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static Elm_Genlist_Item_Class *itc4;

static void
gl4_sel(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   int depth = elm_genlist_item_expanded_depth_get(glit);
   printf("expanded depth for selected item is %d", depth);
}

static void
gl4_exp(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Evas_Object *gl = elm_object_item_widget_get(glit);
   int val = (int)(long) elm_object_item_data_get(glit);
   int i = 0;

   val *= 10;
   for (i = 0; i < 10; i++)
     {
        elm_genlist_item_append(gl, itc4,
                                (void *)(long) (val + i)/* item data */,
                                glit/* parent */,
                                ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                                NULL/* func data */);
     }
}

static void
gl4_con(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
gl4_exp_req(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
gl4_con_req(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

char *gl4_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "Item mode %i", (int)(long)data);
   return strdup(buf);
}

Evas_Object *gl4_content_get(void *data __UNUSED__, Evas_Object *obj, const char *part)
{
   char buf[PATH_MAX];
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *ic = elm_icon_add(obj);
        snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
        elm_image_file_set(ic, buf, NULL);
        evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_show(ic);
        return ic;
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        Evas_Object *ck;
        ck = elm_check_add(obj);
        evas_object_propagate_events_set(ck, EINA_FALSE);
        evas_object_show(ck);
        return ck;
     }
   return NULL;
}

Eina_Bool gl4_state_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return EINA_FALSE;
}

void gl4_del(void *data __UNUSED__, Evas_Object *obj __UNUSED__)
{
   printf("item deleted.\n");
}

static void
_tree_effect_enable_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_tree_effect_enabled_set(data, EINA_TRUE);
}

static void
_tree_effect_disable_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_tree_effect_enabled_set(data, EINA_FALSE);
}

void
test_genlist6(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bx, *fr, *bx2, *rd1, *rd2;

   win = elm_win_util_standard_add("genlist-tree", "Genlist Tree");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   fr = elm_frame_add(win);
   elm_object_text_set(fr, "Genlist Tree Effect");
   elm_box_pack_end(bx, fr);
   evas_object_show(fr);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(fr, bx2);
   evas_object_show(bx2);

   rd1 = elm_radio_add(win);
   elm_radio_state_value_set(rd1, 0);
   elm_object_text_set(rd1, "Enable    ");
   evas_object_show(rd1);
   elm_box_pack_end(bx2, rd1);

   rd2 = elm_radio_add(win);
   elm_radio_state_value_set(rd2, 1);
   elm_radio_group_add(rd1, rd2);
   elm_object_text_set(rd2, "Disable");
   evas_object_show(rd2);
   elm_box_pack_end(bx2, rd2);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);
   elm_genlist_tree_effect_enabled_set(gl, EINA_TRUE);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl4_text_get;
   itc1->func.content_get  = gl4_content_get;
   itc1->func.state_get = gl4_state_get;
   itc1->func.del       = gl4_del;

   itc4 = elm_genlist_item_class_new();
   itc4->item_style     = "tree_effect";
   itc4->func.text_get = gl4_text_get;
   itc4->func.content_get  = gl4_content_get;
   itc4->func.state_get = gl4_state_get;
   itc4->func.del       = gl4_del;

   elm_genlist_item_append(gl, itc4,
                           (void *)1/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc4,
                           (void *)2/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)3/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_NONE, gl4_sel/* func */,
                           NULL/* func data */);

   elm_genlist_item_class_free(itc1);
   elm_genlist_item_class_free(itc4);

   evas_object_smart_callback_add(gl, "expand,request", gl4_exp_req, gl);
   evas_object_smart_callback_add(gl, "contract,request", gl4_con_req, gl);
   evas_object_smart_callback_add(gl, "expanded", gl4_exp, gl);
   evas_object_smart_callback_add(gl, "contracted", gl4_con, gl);

   elm_box_pack_end(bx, gl);

   evas_object_smart_callback_add(rd1, "changed", _tree_effect_enable_cb, gl);
   evas_object_smart_callback_add(rd2, "changed", _tree_effect_disable_cb, gl);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

struct genlist7_data
{
   Evas_Object *win, *naviframe;
};

static Elm_Genlist_Item_Class itc7;

static void
gl_sel7(void *data, Evas_Object *obj, void *event_info)
{
   if (!event_info) return;
   elm_genlist_item_item_class_update(event_info, &itc7);
   printf("sel item data [%p] on genlist obj [%p], item pointer [%p], new item style [%s] \n", data, obj, event_info, itc7.item_style);
}

static void
test_genlist7_back_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   struct genlist7_data *info = data;
   if (!info) return;

   elm_naviframe_item_pop(info->naviframe);
}

static void
test_genlist7_swipe(void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
   struct genlist7_data *info = data;
   Evas_Object *box, *entry, *button;
   char item_data[] = "Just a simple test";

   if ((!event_info) || (!data)) return;

   box = elm_box_add(info->win);
   elm_box_homogeneous_set(box, EINA_FALSE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(box);

   entry = elm_entry_add(info->win);
   elm_entry_scrollable_set(entry, EINA_TRUE);
   elm_entry_editable_set(entry, EINA_FALSE);
   elm_object_text_set(entry, item_data);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(entry);

   button = elm_button_add(info->win);
   elm_object_text_set(button, "back");
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, 0);
   evas_object_smart_callback_add(button, "clicked", test_genlist7_back_cb,
                                  info);
   evas_object_show(button);

   elm_box_pack_start(box, entry);
   elm_box_pack_end(box, button);

   elm_naviframe_item_simple_push(info->naviframe, box);
}

void
test_genlist7(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *naviframe;
   static struct genlist7_data info;
   static Testitem tit[3];

   win = elm_win_util_standard_add("genlist7", "Genlist 7");
   elm_win_autodel_set(win, EINA_TRUE);
   info.win = win;

   naviframe = elm_naviframe_add(win);
   elm_win_resize_object_add(win, naviframe);
   evas_object_size_hint_weight_set(naviframe, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(naviframe, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(naviframe);
   info.naviframe = naviframe;

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(gl, "swipe", test_genlist7_swipe, &info);
   evas_object_show(gl);
   elm_naviframe_item_simple_push(naviframe, gl);

   itc2.item_style     = "default";
   itc2.func.text_get = gl2_text_get;
   itc2.func.content_get  = gl2_content_get;
   itc2.func.state_get = gl2_state_get;
   itc2.func.del       = NULL;

   itc7.item_style     = "double_label";
   itc7.func.text_get = gl5_text_get;
   itc7.func.content_get  = gl5_content_get;
   itc7.func.state_get = gl5_state_get;
   itc7.func.del       = NULL;

   tit[0].mode = 0;
   tit[0].item = elm_genlist_item_append(gl, &itc2,
                                         &(tit[0])/* item data */, NULL/* parent */,
                                         ELM_GENLIST_ITEM_NONE, gl_sel7/* func */,
                                         NULL/* func data */);
   tit[1].mode = 1;
   tit[1].item = elm_genlist_item_append(gl, &itc2,
                                         &(tit[1])/* item data */, NULL/* parent */,
                                         ELM_GENLIST_ITEM_NONE, gl_sel7/* func */,
                                         NULL/* func data */);
   tit[2].mode = 2;
   tit[2].item = elm_genlist_item_append(gl, &itc2,
                                         &(tit[2])/* item data */, NULL/* parent */,
                                         ELM_GENLIST_ITEM_NONE, gl_sel7/* func */,
                                         NULL/* func data */);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static Elm_Genlist_Item_Class itc_group;

char *gl8_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "Group Index # %i (Item # %i)",  (int)((long)data / 10), (int)(long)data);
   return strdup(buf);
}

static void
_bt_show_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_item_show(data, ELM_GENLIST_ITEM_SCROLLTO_TOP);
}

static void
_bt_bring_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_item_bring_in(data, ELM_GENLIST_ITEM_SCROLLTO_TOP);
}

void
test_genlist8(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bt[8], *bx, *bx2, *bx3;
   Elm_Object_Item *gli = NULL, *git = NULL;
   int i, bt_num;

   win = elm_win_util_standard_add("genlist-group", "Genlist Group");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_smart_callback_add(gl, "selected", _gl_selected, NULL);
   evas_object_smart_callback_add(gl, "clicked,double", _gl_double_clicked, NULL);
   evas_object_smart_callback_add(gl, "longpressed", _gl_longpress, NULL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, gl);
   evas_object_show(gl);

   // Use homogeneous mode for group index + homogeneous test.
   // Homogeneous is not mandatory for group index.
   elm_genlist_homogeneous_set(gl, EINA_TRUE);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   itc_group.item_style     = "group_index";
   itc_group.func.text_get = gl8_text_get;
   itc_group.func.content_get  = NULL;
   itc_group.func.state_get = NULL;
   itc_group.func.del       = NULL;

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt_num = 0;
   bt[bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Show 0");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx2, bt[bt_num]);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Show 26");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx2, bt[bt_num]);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Show 101");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx2, bt[bt_num]);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Show 480");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx2, bt[bt_num]);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   bx3 = elm_box_add(win);
   elm_box_horizontal_set(bx3, EINA_TRUE);
   elm_box_homogeneous_set(bx3, EINA_TRUE);
   evas_object_size_hint_weight_set(bx3, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx3, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Bring top 0");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx3, bt[bt_num]);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Bring top 31");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx3, bt[bt_num]);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Bring top 239");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx3, bt[bt_num]);

   bt[++bt_num] = elm_button_add(win);
   elm_object_text_set(bt[bt_num], "Bring top 477");
   evas_object_size_hint_align_set(bt[bt_num], EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt[bt_num], EVAS_HINT_EXPAND, 0.0);
   evas_object_show(bt[bt_num]);
   elm_box_pack_end(bx3, bt[bt_num]);

   elm_box_pack_end(bx, bx3);
   evas_object_show(bx3);

   for (i = 0; i < 500; i++)
     {
        if (!(i % 10))
          {
             gli = git = elm_genlist_item_append(gl, &itc_group,
                                                 (void *)(long)i/* item data */,
                                                 NULL/* parent */,
                                                 ELM_GENLIST_ITEM_GROUP,
                                                 gl_sel/* func */,
                                                 (void *)(long)(i * 10)/* func data */);
             elm_genlist_item_select_mode_set(git, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
          }
        else if (git)
          {
             gli = elm_genlist_item_append(gl, itc1,
                                           (void *)(long)i/* item data */,
                                           git/* parent */,
                                           ELM_GENLIST_ITEM_NONE,
                                           gl_sel/* func */,
                                           (void *)(long)(i * 10)/* func data */);
          }
        switch (i)
          {
           case 0:
              evas_object_smart_callback_add(bt[0], "clicked", _bt_show_cb, gli);
              evas_object_smart_callback_add(bt[4], "clicked", _bt_bring_cb, gli);
              break;
           case 26:
              evas_object_smart_callback_add(bt[1], "clicked", _bt_show_cb, gli);
              break;
           case 31:
              evas_object_smart_callback_add(bt[5], "clicked", _bt_bring_cb, gli);
              break;
           case 101:
              evas_object_smart_callback_add(bt[2], "clicked", _bt_show_cb, gli);
              break;
           case 239:
              evas_object_smart_callback_add(bt[6], "clicked", _bt_bring_cb, gli);
              break;
           case 477:
              evas_object_smart_callback_add(bt[7], "clicked", _bt_bring_cb, gli);
              break;
           case 480:
              evas_object_smart_callback_add(bt[3], "clicked", _bt_show_cb, gli);
              break;
          }
     }
   elm_genlist_item_class_free(itc1);

   evas_object_resize(win, 480, 800);
   evas_object_show(win);
}

/*************/

static void
gl9_exp(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Evas_Object *gl = elm_object_item_widget_get(glit);
   int val = (int)(long) elm_object_item_data_get(glit);
   val *= 10;
   elm_genlist_item_append(gl, itc1,
                           (void *)(long) (val + 1)/* item data */,
                           glit/* parent */,
                           ELM_GENLIST_ITEM_NONE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)(long) (val + 2)/* item data */,
                           glit/* parent */,
                           ELM_GENLIST_ITEM_NONE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)(long) (val + 3)/* item data */,
                           glit/* parent */,
                           ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
}

static void
gl9_con(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
gl9_exp_req(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
gl9_con_req(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

void
test_genlist9(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bx;
   Elm_Object_Item *git;

   win = elm_win_util_standard_add("genlist-group-tree", "Genlist Group Tree");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_smart_callback_add(gl, "selected", _gl_selected, NULL);
   evas_object_smart_callback_add(gl, "clicked,double", _gl_double_clicked, NULL);
   evas_object_smart_callback_add(gl, "longpressed", _gl_longpress, NULL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, gl);
   evas_object_show(gl);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   itc_group.item_style     = "group_index";
   itc_group.func.text_get = gl8_text_get;
   itc_group.func.content_get  = NULL;
   itc_group.func.state_get = NULL;
   itc_group.func.del       = NULL;

   git = elm_genlist_item_append(gl, &itc_group,
                                 (void *)0/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_GROUP, gl4_sel/* func */,
                                 NULL/* func data */);
   elm_genlist_item_select_mode_set(git, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

   elm_genlist_item_append(gl, itc1,
                           (void *)1/* item data */, git/* parent */, ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)2/* item data */, git/* parent */, ELM_GENLIST_ITEM_NONE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)3/* item data */, git/* parent */, ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
   git = elm_genlist_item_append(gl, &itc_group,
                                 (void *)4/* item data */, NULL/* parent */, ELM_GENLIST_ITEM_GROUP, gl4_sel/* func */,
                                 NULL/* func data */);
   elm_genlist_item_select_mode_set(git, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

   elm_genlist_item_append(gl, itc1,
                           (void *)5/* item data */, git/* parent */, ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)6/* item data */, git/* parent */, ELM_GENLIST_ITEM_NONE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_append(gl, itc1,
                           (void *)7/* item data */, git/* parent */, ELM_GENLIST_ITEM_TREE, gl4_sel/* func */,
                           NULL/* func data */);
   elm_genlist_item_class_free(itc1);

   evas_object_smart_callback_add(gl, "expand,request", gl9_exp_req, gl);
   evas_object_smart_callback_add(gl, "contract,request", gl9_con_req, gl);
   evas_object_smart_callback_add(gl, "expanded", gl9_exp, gl);
   evas_object_smart_callback_add(gl, "contracted", gl9_con, gl);

   evas_object_resize(win, 480, 800);
   evas_object_show(win);
}

/*************/

static Elm_Genlist_Item_Class itc10;
static char *mode_type[] = { "slide", "rotate" };

char *gl10_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part)
{
   char buf[256];
   if (!strcmp(part, "elm.text.mode"))
     snprintf(buf, sizeof(buf), "Mode # %i", (int)(long)data);
   else
     snprintf(buf, sizeof(buf), "Item # %i", (int)(long)data);
   return strdup(buf);
}

Evas_Object *gl10_content_get(void *data __UNUSED__, Evas_Object *obj, const char *part)
{
   char buf[PATH_MAX];
   Evas_Object *ic = elm_icon_add(obj);
   if (!strcmp(part, "elm.swallow.end"))
     snprintf(buf, sizeof(buf), "%s/images/bubble.png", elm_app_data_dir_get());
   else
     snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
   elm_image_file_set(ic, buf, NULL);
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
_gl_sel10(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   if (!data) return;
   int v = elm_radio_value_get(data);
   if (v == 1)
     elm_genlist_item_decorate_mode_set(event_info, mode_type[v], EINA_TRUE);
}

static void
_my_gl_mode_right(void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
   if (!data) return;
   int v = elm_radio_value_get(data);
   if (v == 0)
     elm_genlist_item_decorate_mode_set(event_info, mode_type[v], EINA_TRUE);
}

static void
_my_gl_mode_left(void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
   if (!data) return;
   int v = elm_radio_value_get(data);
   if (v == 0)
     elm_genlist_item_decorate_mode_set(event_info, mode_type[v], EINA_FALSE);
}

static void
_my_gl_mode_cancel(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   fprintf(stderr, "drag\n");
   if (!data) return;
   int v = elm_radio_value_get(data);
   Elm_Object_Item *glit = elm_genlist_decorated_item_get(obj);
   if (glit) elm_genlist_item_decorate_mode_set(glit, mode_type[v], EINA_FALSE);
}

void
test_genlist10(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *fr, *bx, *bx2, *rd, *rdg, *gl;
   int i;

   win = elm_win_util_standard_add("genlist-decorate-item-mode", "Genlist Decorate Item Mode");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   fr = elm_frame_add(win);
   elm_object_text_set(fr, "Decorate Mode Type");
   elm_box_pack_end(bx, fr);
   evas_object_show(fr);

   bx2 = elm_box_add(win);
   elm_object_content_set(fr, bx2);
   evas_object_show(bx2);

   rd = elm_radio_add(win);
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_radio_state_value_set(rd, 0);
   elm_object_text_set(rd, "Slide : Sweep genlist items to the right.");
   evas_object_show(rd);
   elm_box_pack_end(bx2, rd);
   rdg = rd;

   rd = elm_radio_add(win);
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_radio_state_value_set(rd, 1);
   elm_object_text_set(rd, "Rotate : Click each item.");
   elm_radio_group_add(rd, rdg);
   evas_object_show(rd);
   elm_box_pack_end(bx2, rd);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(gl, "drag,start,right", _my_gl_mode_right, rdg);
   evas_object_smart_callback_add(gl, "drag,start,left", _my_gl_mode_left, rdg);
   evas_object_smart_callback_add(gl, "drag,start,up", _my_gl_mode_cancel, rdg);
   evas_object_smart_callback_add(gl, "drag,start,down", _my_gl_mode_cancel, rdg);
   evas_object_show(gl);

   itc10.item_style     = "default";
   itc10.decorate_item_style = "mode";
   itc10.func.text_get = gl10_text_get;
   itc10.func.content_get  = gl10_content_get;
   itc10.func.state_get = gl_state_get;
   itc10.func.del       = NULL;

   for (i = 0; i < 50; i++)
     elm_genlist_item_append(gl,
                             &itc10,
                             (void *)(long)(1000 + i)/* item data */,
                             NULL/* parent */,
                             ELM_GENLIST_ITEM_NONE/* flags */,
                             _gl_sel10/* func */,
                             rdg/* func data */);

   elm_box_pack_end(bx, gl);

   evas_object_resize(win, 520, 520);
   evas_object_show(win);
}

/*************/

static void
_reorder_tg_changed_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   elm_genlist_reorder_mode_set(data, elm_check_state_get(obj));
}

/**
 * gl_moved is called after an item was reordered.
 * This is only called when reorder mode is enabled.
 *
 * @param data         :  the genlist object passed as data.
 * @param obj          :  the genlist object.
 * @param item         :  the moved item.
 *
 *  the item(*item) had been moved
 *
 */
static void gl_moved(Evas_Object *data __UNUSED__, Evas_Object *obj __UNUSED__, Elm_Object_Item *item __UNUSED__)
{
   // if needed, add application logic.
}

/**
 * gl_moved_after is called after an item was reordered.
 * This is only called when reorder mode is enabled.
 *
 * @param data         :  the genlist object passed as data.
 * @param obj          :  the genlist object.
 * @param item         :  the moved item.
 *
 *  the item(*item) had been moved after the given relative item(*rel_item) in list.
 *
 */
static void gl_moved_after(Evas_Object *data __UNUSED__, Evas_Object *obj __UNUSED__, Elm_Object_Item *item __UNUSED__)
{
   // if needed, add application logic.
   Elm_Object_Item *it;
   it = elm_genlist_item_prev_get(item);
   printf("it=%p, prev_it=%p\n",item,it);
}

/**
 * gl_moved_before is called after an item was reordered.
 * This is only called when reorder mode is enabled.
 *
 * @param data         :  the genlist object passed as data.
 * @param obj          :  the genlist object.
 * @param item         :  the moved item.
 *
 *  the item(*item) had been moved before the given relative item(*rel_item) in list.
 *
 */
static void gl_moved_before(Evas_Object *data __UNUSED__, Evas_Object *obj __UNUSED__, Elm_Object_Item *item __UNUSED__)
{
   // if needed, add application logic.
   Elm_Object_Item *it;
   it = elm_genlist_item_next_get(item);
   printf("it=%p, next_it=%p\n",item,it);
}

void
test_genlist11(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *fr, *lb, *bx, *tg, *gl;
   int i;

   win = elm_win_util_standard_add("genlist-reorder-mode", "Genlist Reorder Mode");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   fr = elm_frame_add(win);

   elm_object_text_set(fr, "Reorder Mode");
   elm_box_pack_end(bx, fr);
   evas_object_show(fr);

   lb = elm_label_add(win);
   elm_object_text_set(lb,
                       "Enable reorder mode if you want to move item.<br/>"
                       "Then long press and drag item.");
   elm_object_content_set(fr, lb);
   evas_object_show(lb);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   tg = elm_check_add(win);
   elm_object_style_set(tg, "toggle");
   elm_object_text_set(tg, "Reorder Mode:");
   elm_check_state_set(tg, elm_config_mirrored_get());
   evas_object_smart_callback_add(tg, "changed", _reorder_tg_changed_cb, gl);
   elm_box_pack_end(bx, tg);
   evas_object_show(tg);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;
   evas_object_smart_callback_add(gl, "moved", (Evas_Smart_Cb)gl_moved, gl);
   evas_object_smart_callback_add(gl, "moved,after", (Evas_Smart_Cb)gl_moved_after, gl);
   evas_object_smart_callback_add(gl, "moved,before", (Evas_Smart_Cb)gl_moved_before, gl);

   for (i = 0; i < 50; i++)
     elm_genlist_item_append(gl,
                             itc1,
                             (void *)(long)(1 + i)/* item data */,
                             NULL/* parent */,
                             ELM_GENLIST_ITEM_NONE/* flags */,
                             NULL/* func */,
                             NULL/* func data */);

   elm_genlist_item_class_free(itc1);

   elm_box_pack_end(bx, gl);

   evas_object_resize(win, 400, 500);
   evas_object_show(win);
}

/*************/

char *gl12_text_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return strdup("Enlightenment is not just a window manager for Linux/X11 and others, but also a whole suite of libraries to help you create beautiful user interfaces with much less work than doing it the old fashioned way and fighting with traditional toolkits, not to mention a traditional window manager. It covers uses from small mobile devices like phones all the way to powerful multi-core desktops (which are the primary development environment).");
}

void
test_genlist12(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *bx, *gl;
   int i;

   win = elm_win_util_standard_add("genlist-textblock", "Genlist Textblock");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   elm_genlist_mode_set(gl, ELM_LIST_COMPRESS);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, gl);
   evas_object_show(gl);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "message";
   itc1->func.text_get = gl12_text_get;
   itc1->func.content_get  = gl_content_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   for (i = 0; i < 1000; i++)
     {
        elm_genlist_item_append(gl, itc1,
                                (void *)(long)i/* item data */,
                                NULL/* parent */,
                                ELM_GENLIST_ITEM_NONE,
                                gl_sel/* func */,
                                (void *)(long)(i * 10)/* func data */);
     }
   elm_genlist_item_class_free(itc1);

   evas_object_resize(win, 400, 500);
   evas_object_show(win);
}

/*************/

static int
gl13_cmp(const void *pa, const void *pb)
{
   const Elm_Object_Item *ia = pa, *ib = pb;
   int a = (int)(long) elm_object_item_data_get(ia);
   int b = (int)(long) elm_object_item_data_get(ib);
   return a - b;
}

void
test_genlist13(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Elm_Object_Item *pi[6];
   Evas_Object *win, *bx, *gl; int i, base, idx[3] = {1, 10, 15};

   win = elm_win_util_standard_add("genlist-tree-insert-sorted", "Genlist Tree, Insert Sorted");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   itc4 = elm_genlist_item_class_new();
   itc4->item_style = "default";
   itc4->func.text_get = gl4_text_get;
   itc4->func.content_get = NULL;
   itc4->func.state_get = NULL;
   itc4->func.del = NULL;

   /* mixed order to test insert sorted */

   for (i = 0; i < 3; i++)
     {
        pi[i] = elm_genlist_item_sorted_insert
          (gl, itc4, (void *)(long)idx[i]/* item data */, NULL/* parent */,
           ELM_GENLIST_ITEM_TREE, gl13_cmp/* cmp */,
           NULL/* func */, NULL/* func data */);
     }

   for (i = 0, base = 100; i < 3; i++, base += 100)
     {
        int j;
        for (j = 0; j < 3; j++)
          {
             elm_genlist_item_sorted_insert
               (gl, itc4, (void *)(long)(idx[j] + base)/* item data */,
                pi[i]/* parent */, ELM_GENLIST_ITEM_NONE,
                gl13_cmp/* cmp */, NULL/* func */, NULL/* func data */);
          }
     }

   for (i = 0; i < 3; i++)
     {
        pi[i + 3] = elm_genlist_item_sorted_insert
          (gl, itc4, (void *)(long)(idx[i] * 2)/* item data */, NULL/* parent */,
           ELM_GENLIST_ITEM_TREE, gl13_cmp/* cmp */, NULL/* func */,
           NULL/* func data */);
     }


   for (i = 0, base = 10000; i < 3; i++, base += 10000)
     {
        int j;
        for (j = 0; j < 3; j++)
          {
             elm_genlist_item_sorted_insert
               (gl, itc4, (void *)(long)(idx[j] + base)/* item data */,
                pi[i + 3]/* parent */, ELM_GENLIST_ITEM_NONE,
                gl13_cmp/* cmp */, NULL/* func */, NULL/* func data */);
          }
     }
   elm_genlist_item_class_free(itc4);
   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static void
my_gl_insert_before_rel(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   static int i = 1000;
   Elm_Object_Item *gli_selected;

   if (!itc1) itc1 = elm_genlist_item_class_new();
   itc1->item_style = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get = NULL;
   itc1->func.state_get = NULL;
   itc1->func.del = NULL;

   gli_selected = elm_genlist_selected_item_get(gl);
   if (!gli_selected)
     {
        printf("no item selected\n");
        return;
     }

   elm_genlist_item_insert_before(gl, itc1,
                                  (void *)(long)i/* item data */,
                                  elm_genlist_item_parent_get(gli_selected),
                                  gli_selected/* item before */,
                                  ELM_GENLIST_ITEM_NONE,
                                  NULL/* func */, NULL/* func data */);
   i++;
}

static void
my_gl_insert_after_rel(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *gl = data;
   static int i = 0;
   Elm_Object_Item *gli_selected;

   if (!itc1) itc1 = elm_genlist_item_class_new();
   itc1->item_style = "default";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get = NULL;
   itc1->func.state_get = NULL;
   itc1->func.del = NULL;

   gli_selected = elm_genlist_selected_item_get(gl);
   if (!gli_selected)
     {
        printf("no item selected\n");
        return;
     }

   elm_genlist_item_insert_after(gl, itc1,
                                  (void *)(long)i/* item data */,
                                  elm_genlist_item_parent_get(gli_selected),
                                  gli_selected/* item after */,
                                  ELM_GENLIST_ITEM_NONE,
                                  NULL/* func */, NULL/* func data */);
   i++;
}

void
test_genlist14(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Elm_Object_Item *pi[6];
   Evas_Object *win, *bx, *bx2, *bt, *gl;
   int i, base;
   /* index: 0,  1,  2,  3,  4,  5
    * items: 1, 15, 11,  2, 33, 21
    * final: 1,  2, 11, 15, 21, 33
    */
   const int idx[6] = {1, 15, 11, 2, 33, 21};
   const int relative[6] = {
      0, /* item  '1' -  not used */
      0, /* item '15' - insert after pi[0], item '1' */
     -1, /* item '11' - insert before pi[1], item '15' */
     -2, /* item  '2' - insert before pi[2], item '11' */
      1, /* item '33' - insert after pi[1], item '15' */
     -4  /* item '21' - insert before pi[4], item '33' */
   };

   win = elm_win_util_standard_add("genlist-tree-insert-relative", "Genlist Tree, Insert Relative");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   itc4 = elm_genlist_item_class_new();
   itc4->item_style = "default";
   itc4->func.text_get = gl4_text_get;
   itc4->func.content_get = NULL;
   itc4->func.state_get = NULL;
   itc4->func.del = NULL;

   /* mixed order to test insert sorted */

   // BUG1: should be able to insert before=NULL, prepend
   // BUG2: should be able to insert after=NULL, append

   pi[0] = elm_genlist_item_append
     (gl, itc4, (void *)(long)idx[0]/* item data */,
      NULL/* parent */,
      ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);

   for (i = 1; i < 3; i++)
     {
        if (relative[i] < 0)
          pi[i] = elm_genlist_item_insert_before
            (gl, itc4, (void *)(long)idx[i]/* item data */,
             NULL/* parent */, pi[-relative[i]],
             ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
        else
          pi[i] = elm_genlist_item_insert_after
            (gl, itc4, (void *)(long)idx[i]/* item data */,
             NULL/* parent */, pi[relative[i]],
             ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
     }

   for (i = 0; i < 3; i++)
     {
        Elm_Object_Item *sub_glit[6];
        int j;

        base = 1000 * (int)(long) elm_object_item_data_get(pi[i]);

        sub_glit[0] = elm_genlist_item_append
          (gl, itc4, (void *)(long) (idx[0] + base)/* item data */, pi[i]/* parent */,
           ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);

        for (j = 1; j < 6; j++) {
           if (relative[j] < 0)
             sub_glit[j] = elm_genlist_item_insert_before
               (gl, itc4, (void *)(long) (idx[j] + base)/* item data */,
                pi[i]/* parent */, sub_glit[-relative[j]],
                ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
           else
             sub_glit[j] = elm_genlist_item_insert_after
               (gl, itc4, (void *)(long) (idx[j] + base)/* item data */,
                pi[i]/* parent */, sub_glit[relative[j]],
                ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
        }
     }

   for (i = 3; i < 6; i++)
     {
        if (relative[i] < 0)
          pi[i] = elm_genlist_item_insert_before
            (gl, itc4, (void *)(long) idx[i]/* item data */,
             NULL/* parent */, pi[-relative[i]],
             ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
        else
          pi[i] = elm_genlist_item_insert_after
            (gl, itc4, (void *)(long) idx[i]/* item data */,
             NULL/* parent */, pi[relative[i]],
             ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
     }

   for (i = 3; i < 6; i++)
     {
        Elm_Object_Item *sub_glit[6];
        int j;

        base = 1000 * (int)(long) elm_object_item_data_get(pi[i]);

        sub_glit[0] = elm_genlist_item_append
          (gl, itc4, (void *)(long) (idx[0] + base)/* item data */, pi[i]/* parent */,
           ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);

        for (j = 1; j < 6; j++) {
           if (relative[j] < 0)
             sub_glit[j] = elm_genlist_item_insert_before
               (gl, itc4, (void *)(long) (idx[j] + base)/* item data */,
                pi[i]/* parent */, sub_glit[-relative[j]],
                ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
           else
             sub_glit[j] = elm_genlist_item_insert_after
               (gl, itc4, (void *)(long) (idx[j] + base)/* item data */,
                pi[i]/* parent */, sub_glit[relative[j]],
                ELM_GENLIST_ITEM_TREE, NULL/* func */, NULL/* func data */);
        }
     }

   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "+ before");
   evas_object_smart_callback_add(bt, "clicked", my_gl_insert_before_rel, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "+ after");
   evas_object_smart_callback_add(bt, "clicked", my_gl_insert_after_rel, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   /* item_class_ref is needed for itc4. some items can be added in callbacks */
   elm_genlist_item_class_ref(itc4);
   elm_genlist_item_class_free(itc4);

   evas_object_resize(win, 320, 320);
   evas_object_show(win);
}

/*************/

static void _edit_icon_clicked_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Elm_Object_Item *item = (Elm_Object_Item *)data;
   elm_object_item_del(item);
}

char *gl15_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   const Testitem *tit = data;
   char buf[256];
   snprintf(buf, sizeof(buf), "Item #%i", tit->mode);
   return strdup(buf);
}

Evas_Object *gl15_content_get(void *data, Evas_Object *obj, const char *part)
{
   Testitem *tit = data;
   char buf[PATH_MAX];

   // "edit_default" EDC layout is like below. each part is swallow part.
   // the existing item is swllowed to elm.swallow.edit.content part.
   // --------------------------------------------------------------------
   // | elm.edit.icon.1 | elm.swallow.decorate.content | elm.edit.icon,2 |
   // --------------------------------------------------------------------

   if (!strcmp(part, "elm.swallow.end"))
     {
        Evas_Object *ic = elm_icon_add(obj);
        snprintf(buf, sizeof(buf), "%s/images/bubble.png", PACKAGE_DATA_DIR);
        elm_image_file_set(ic, buf, NULL);
        evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        return ic;
     }
   else if (!strcmp(part, "elm.edit.icon.1"))
     {
        Evas_Object *ck;
        ck = elm_check_add(obj);
        elm_check_state_pointer_set(ck, &tit->checked);
        evas_object_propagate_events_set(ck, EINA_FALSE);
        evas_object_show(ck);
        return ck;
     }
   else if (!strcmp(part, "elm.edit.icon.2"))
     {
        Evas_Object *icn = elm_icon_add(obj);
        snprintf(buf, sizeof(buf), "%s/images/icon_06.png", PACKAGE_DATA_DIR);
        elm_image_file_set(icn, buf, NULL);
        evas_object_propagate_events_set(icn, EINA_FALSE);
        evas_object_size_hint_aspect_set(icn, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_smart_callback_add(icn, "clicked", _edit_icon_clicked_cb, (void *)tit->item);
        return icn;
     }
   else return NULL;
}

static void
gl15_sel(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Testitem *tit = data;
   if (elm_genlist_decorate_mode_get(obj))
     {
        if (!tit->checked) tit->checked = EINA_TRUE;
        else tit->checked = EINA_FALSE;
     }
   elm_genlist_item_update(tit->item);
}

static void
gl15_deco_all_mode(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_decorate_mode_set(data, EINA_TRUE);
   elm_genlist_select_mode_set(data, ELM_OBJECT_SELECT_MODE_ALWAYS);
}

static void
gl15_normal_mode(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_genlist_decorate_mode_set(data, EINA_FALSE);
   elm_genlist_select_mode_set(data, ELM_OBJECT_SELECT_MODE_DEFAULT);
}

void gl15_del(void *data __UNUSED__, Evas_Object *obj __UNUSED__)
{
   printf("item deleted.\n");
}

static Elm_Genlist_Item_Class *itc15;
void
test_genlist15(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *bx, *bx2, *bt, *gl;
   int i;
   static Testitem tit[100];

   win = elm_win_util_standard_add("genlist-decorate-all-mode", "Genlist Decorate All Mode");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   itc15 = elm_genlist_item_class_new();
   itc15->item_style     = "default";
   itc15->func.text_get = gl15_text_get;
   itc15->func.content_get  = gl15_content_get;
   itc15->func.state_get = gl_state_get;
   itc15->func.del       = gl15_del;
   itc15->decorate_all_item_style = "edit";

   for (i = 0; i < 100; i++)
     {
        tit[i].mode = i;
        tit[i].item = elm_genlist_item_append(gl, itc15,
                                              &(tit[i])/* item data */,
                                              NULL/* parent */,
                                              ELM_GENLIST_ITEM_NONE/* flags */,
                                              gl15_sel/* func */,
                                              (void *)(long)&(tit[i])/* func data */);
     }
   elm_genlist_item_class_free(itc15);
   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   bx2 = elm_box_add(win);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_homogeneous_set(bx2, EINA_TRUE);
   evas_object_size_hint_weight_set(bx2, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Decorate All mode");
   evas_object_smart_callback_add(bt, "clicked", gl15_deco_all_mode, gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Normal mode");
   evas_object_smart_callback_add(bt, "clicked", gl15_normal_mode,gl);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(bx2, bt);
   evas_object_show(bt);

   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   evas_object_resize(win, 520, 520);
   evas_object_show(win);
}

/*************/

static void _flip_icon_clicked_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   const Testitem *tit = data;

   if (elm_genlist_item_flip_get(tit->item))
     elm_genlist_item_flip_set(tit->item, EINA_FALSE);
   else
     elm_genlist_item_flip_set(tit->item, EINA_TRUE);
}

char *gl16_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   const Testitem *tit = data;
   char buf[256];
   snprintf(buf, sizeof(buf), "Item #%i", tit->mode);
   return strdup(buf);
}

Evas_Object *gl16_content_get(void *data, Evas_Object *obj, const char *part)
{
   Testitem *tit = data;
   char buf[PATH_MAX];

   if (!strcmp(part, "elm.text.flip"))
     {
        Evas_Object *btn = elm_button_add(obj);
        elm_object_text_set(btn, "flipped content placement");
        evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, 0.0);
        evas_object_smart_callback_add(btn, "clicked", _flip_icon_clicked_cb, (void *)tit);
        evas_object_show(btn);
        return btn;
     }
   else if (!strcmp(part, "elm.edit.icon.1"))
     {
        Evas_Object *icn = elm_icon_add(obj);
        snprintf(buf, sizeof(buf), "%s/images/icon_04.png", PACKAGE_DATA_DIR);
        elm_image_file_set(icn, buf, NULL);
        evas_object_propagate_events_set(icn, EINA_FALSE);
        evas_object_size_hint_aspect_set(icn, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        return icn;
     }
   else if (!strcmp(part, "elm.edit.icon.2"))
     {
        Evas_Object *icn = elm_icon_add(obj);
        snprintf(buf, sizeof(buf), "%s/images/icon_09.png", PACKAGE_DATA_DIR);
        elm_image_file_set(icn, buf, NULL);
        evas_object_propagate_events_set(icn, EINA_FALSE);
        evas_object_size_hint_aspect_set(icn, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_smart_callback_add(icn, "clicked", _flip_icon_clicked_cb, (void *)tit);
        return icn;
     }
   else return NULL;
}

void
test_genlist16(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *bx, *gl;
   int i;
   static Testitem tit[100];

   win = elm_win_util_standard_add("genlist-flip-mode", "Genlist Flip Mode");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   /* note: flip mode can be used with/without edit mode */
   elm_genlist_decorate_mode_set(gl, EINA_TRUE);
   elm_genlist_select_mode_set(gl, ELM_OBJECT_SELECT_MODE_ALWAYS);

   itc15 = elm_genlist_item_class_new();
   itc15->item_style     = "default";
   itc15->func.text_get = gl16_text_get;
   itc15->func.content_get = gl16_content_get;
   itc15->func.state_get = gl_state_get;
   itc15->func.del       = NULL;
   itc15->decorate_all_item_style = "edit";

   for (i = 0; i < 100; i++)
     {
        tit[i].mode = i;
        tit[i].item = elm_genlist_item_append(gl, itc15,
                                              &(tit[i])/* item data */,
                                              NULL/* parent */,
                                              ELM_GENLIST_ITEM_NONE/* flags */,
                                              gl_sel/* func */,
                                              (void *)(long)&(tit[i])/* func data */);
     }
   elm_genlist_item_class_free(itc15);
   elm_box_pack_end(bx, gl);
   evas_object_show(bx);

   evas_object_resize(win, 520, 520);
   evas_object_show(win);
}

/*************/

static Elm_Genlist_Item_Class *itc17;

char *
gl17_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part)
{
   const Testitem *tit = data;
   char buf[256];
   if (!strcmp(part, "elm.text.mode"))
     snprintf(buf, sizeof(buf), "Mode # %i", tit->mode);
   else
     snprintf(buf, sizeof(buf), "Item # %i", (int)(long)data);
   return strdup(buf);
}

Evas_Object *
_decorate_item_mode_frame_new(Evas_Object *win, Evas_Object **rdg)
{
   Evas_Object *fr, *bx, *rd;

   fr = elm_frame_add(win);
   evas_object_size_hint_weight_set(fr, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(fr, "Decorate Item Mode Type");
   evas_object_show(fr);

   bx = elm_box_add(win);
   elm_object_content_set(fr, bx);
   evas_object_show(bx);

   rd = elm_radio_add(win);
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_radio_state_value_set(rd, 0);
   elm_object_text_set(rd, "Slide : Sweep genlist items to the right.");
   evas_object_show(rd);
   elm_box_pack_end(bx, rd);
   *rdg = rd;

   rd = elm_radio_add(win);
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_radio_state_value_set(rd, 1);
   elm_object_text_set(rd, "Rotate : Click each item.");
   elm_radio_group_add(rd, *rdg);
   evas_object_show(rd);
   elm_box_pack_end(bx, rd);

   return fr;
}

void
_decorate_all_set_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   if (!data) return;

   Evas_Object *gl;
   gl = evas_object_data_get(obj, "gl");
   if (!gl) return;

   if (evas_object_data_get(gl, "tree_effect"))
     elm_genlist_tree_effect_enabled_set(gl, EINA_FALSE);
   elm_genlist_decorate_mode_set(gl, EINA_TRUE);
}

void
_decorate_all_unset_btn_clicked_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   if (evas_object_data_get(data, "tree_effect"))
     elm_genlist_tree_effect_enabled_set(data, EINA_TRUE);
   elm_genlist_decorate_mode_set(data, EINA_FALSE);
}

Evas_Object *
_decorate_all_mode_frame_new(Evas_Object *win, Evas_Object *rdg, Evas_Object *gl)
{
   Evas_Object *fr, *bx, *btn;

   fr = elm_frame_add(win);
   evas_object_size_hint_weight_set(fr, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(fr, "Decorate All Mode Type");
   evas_object_show(fr);

   bx = elm_box_add(win);
   elm_object_content_set(fr, bx);
   evas_object_show(bx);

   btn = elm_button_add(win);
   evas_object_data_set(btn, "gl", gl);
   elm_object_text_set(btn, "Set Decorate All Mode");
   evas_object_smart_callback_add(btn, "clicked", _decorate_all_set_btn_clicked_cb, rdg);
   elm_box_pack_end(bx, btn);
   evas_object_show(btn);

   btn = elm_button_add(win);
   elm_object_text_set(btn, "Unset Decorate All Mode");
   evas_object_smart_callback_add(btn, "clicked", _decorate_all_unset_btn_clicked_cb, gl);
   elm_box_pack_end(bx, btn);
   evas_object_show(btn);

   return fr;
}

void
test_genlist17(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *fr1, *fr2, *bx, *bx2, *rdg = NULL, *gl;
   int i;
   static Testitem tit[50];

   win = elm_win_util_standard_add("genlist-decorate-modes", "Genlist Decorate Modes");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   bx2 = elm_box_add(win);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   fr1 = _decorate_item_mode_frame_new(win, &rdg);
   elm_box_pack_end(bx2, fr1);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(gl, "drag,start,right", _my_gl_mode_right, rdg);
   evas_object_smart_callback_add(gl, "drag,start,left", _my_gl_mode_left, rdg);
   evas_object_smart_callback_add(gl, "drag,start,up", _my_gl_mode_cancel, rdg);
   evas_object_smart_callback_add(gl, "drag,start,down", _my_gl_mode_cancel, rdg);
   evas_object_show(gl);

   itc17 = elm_genlist_item_class_new();
   itc17->item_style     = "default";
   itc17->decorate_item_style = "mode";
   itc17->func.text_get = gl15_text_get;
   itc17->func.content_get  = gl15_content_get;
   itc17->func.state_get = gl_state_get;
   itc17->func.del       = NULL;
   itc17->decorate_all_item_style = "edit";

   for (i = 0; i < 50; i++)
     {
        tit[i].checked = EINA_FALSE;
        tit[i].mode = i;
        tit[i].item = elm_genlist_item_append(gl,
                                              itc17,
                                              &(tit[i])/* item data */,
                                              NULL/* parent */,
                                              ELM_GENLIST_ITEM_NONE/* flags */,
                                              NULL/* select func */,
                                              NULL/* func data */);
     }

   elm_genlist_item_class_free(itc17);
   elm_box_pack_end(bx, gl);

   fr2 = _decorate_all_mode_frame_new(win, rdg, gl);
   elm_box_pack_end(bx2, fr2);

   evas_object_resize(win, 520, 520);
   evas_object_show(win);
}

/*************/

Elm_Genlist_Item_Class *itc18, *itc18_tree;
Evas_Object *
_tree_effect_frame_new(Evas_Object *win, Evas_Object **rdg, Evas_Object *gl)
{
   Evas_Object *fr, *bx, *rd;

   fr = elm_frame_add(win);
   elm_object_text_set(fr, "Genlist Tree Effect");
   evas_object_size_hint_weight_set(fr, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(fr);

   bx = elm_box_add(win);
   elm_box_homogeneous_set(bx, EINA_TRUE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(fr, bx);
   evas_object_show(bx);

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, 0);
   elm_object_text_set(rd, "Enable");
   evas_object_smart_callback_add(rd, "changed", _tree_effect_enable_cb, gl);
   evas_object_show(rd);
   elm_box_pack_end(bx, rd);
   *rdg = rd;

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, 1);
   elm_radio_group_add(rd, *rdg);
   elm_object_text_set(rd, "Disable");
   evas_object_smart_callback_add(rd, "changed", _tree_effect_disable_cb, gl);
   evas_object_show(rd);
   elm_box_pack_end(bx, rd);


   return fr;
}

static void
gl18_sel(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   int depth = elm_genlist_item_expanded_depth_get(glit);
   printf("expanded depth for selected item is %d\n", depth);
}

static void
gl18_del(void *data, Evas_Object *obj __UNUSED__)
{
   free(data);
}

static void
gl18_exp(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Evas_Object *gl = elm_object_item_widget_get(glit);
   Testitem *tit = elm_object_item_data_get(glit);
   int i, j, val;

   val = tit->mode * 10;
   for (i = 0; i < 100; i++)
     {
        tit = calloc(1, sizeof(Testitem));
        tit->mode = val + i;
        tit->item = elm_genlist_item_append(gl,
                                            itc18_tree,
                                            tit/* item data */,
                                            glit/* parent */,
                                            ELM_GENLIST_ITEM_TREE,
                                            gl18_sel/* func */,
                                            NULL/* func data */);
     }
   for (j = 0; j < 3; j++)
     {
        tit = calloc(1, sizeof(Testitem));
        tit->mode = i;
        tit->item = elm_genlist_item_append(gl,
                                            itc18,
                                            tit/* item data */,
                                            glit/* parent */,
                                            ELM_GENLIST_ITEM_NONE,
                                            gl18_sel/* func */,
                                            NULL/* func data */);
        i++;
     }
}

static void
gl18_con(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
gl18_exp_req(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
gl18_con_req(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

void
test_genlist18(void        *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void        *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bx, *fr1, *fr2, *bx2, *rdg;
   Testitem *tit = NULL;
   int i, j;

   win = elm_win_util_standard_add("genlist-tree-decorate-all",
                                   "Genlist Tree and Decorate All Mode");
   elm_win_autodel_set(win, EINA_TRUE);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   bx2 = elm_box_add(win);
   evas_object_size_hint_align_set(bx2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(bx2, EINA_TRUE);
   elm_box_pack_end(bx, bx2);
   evas_object_show(bx2);

   gl = elm_genlist_add(win);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(gl);

   elm_genlist_tree_effect_enabled_set(gl, EINA_TRUE);
   evas_object_data_set(gl, "tree_effect", "true");

   /* normal item */
   itc18 = elm_genlist_item_class_new();
   itc18->item_style     = "tree_effect"; /* item/tree_effect */
   itc18->func.text_get = gl15_text_get;
   itc18->func.content_get  = gl15_content_get;
   itc18->func.state_get = gl_state_get;
   itc18->func.del       = gl18_del;
   itc18->decorate_all_item_style = "edit";

   /* expandable item */
   itc18_tree = elm_genlist_item_class_new();
   itc18_tree->item_style     = "tree_effect"; /* tree/tree_effect */
   itc18_tree->func.text_get = gl15_text_get;
   itc18_tree->func.content_get  = gl15_content_get;
   itc18_tree->func.state_get = gl_state_get;
   itc18_tree->func.del       = gl18_del;
   itc18_tree->decorate_all_item_style = "edit";

   for (i = 0; i < 5; i++)
     {
        tit = calloc(1, sizeof(Testitem));
        tit->mode = i;
        tit->item = elm_genlist_item_append(gl,
                                            itc18_tree,
                                            tit/* item data */,
                                            NULL/* parent */,
                                            ELM_GENLIST_ITEM_TREE,
                                            gl18_sel/* func */,
                                            NULL/* func data */);
     }

   for (j = 0; j < 3; j++)
     {
        tit = calloc(1, sizeof(Testitem));
        tit->mode = i;
        tit->item = elm_genlist_item_append(gl,
                                            itc18,
                                            tit/* item data */,
                                            NULL/* parent */,
                                            ELM_GENLIST_ITEM_NONE,
                                            gl18_sel/* func */,
                                            NULL/* func data */);
        i++;
     }

   elm_genlist_item_class_free(itc18);
   elm_genlist_item_class_free(itc18_tree);

   evas_object_smart_callback_add(gl, "expand,request", gl18_exp_req, gl);
   evas_object_smart_callback_add(gl, "contract,request", gl18_con_req, gl);
   evas_object_smart_callback_add(gl, "expanded", gl18_exp, gl);
   evas_object_smart_callback_add(gl, "contracted", gl18_con, gl);

   elm_box_pack_end(bx, gl);

   fr1 = _tree_effect_frame_new(win, &rdg, gl);
   elm_box_pack_end(bx2, fr1);

   fr2 = _decorate_all_mode_frame_new(win, rdg, gl);
   elm_box_pack_end(bx2, fr2);

   evas_object_resize(win, 400, 800);
   evas_object_show(win);
}

void
test_genlist19(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *win, *gl, *bt_50, *bt_1500, *bx, *bxx, *bt;
   Evas_Object *over;
   Elm_Object_Item *gli;
   int i;
   api_data *api = calloc(1, sizeof(api_data));

   win = elm_win_util_standard_add("genlist-full", "Genlist Full");
   elm_win_autodel_set(win, EINA_TRUE);
   evas_object_event_callback_add(win, EVAS_CALLBACK_FREE, _cleanup_cb, api);

   bxx = elm_box_add(win);
   elm_win_resize_object_add(win, bxx);
   evas_object_size_hint_weight_set(bxx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bxx);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   api->box = bx;
   evas_object_show(bx);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Next API function");
   evas_object_smart_callback_add(bt, "clicked", _api_bt_clicked, (void *) api);
   elm_box_pack_end(bxx, bt);
   elm_object_disabled_set(bt, api->state == API_STATE_LAST);
   evas_object_show(bt);

   elm_box_pack_end(bxx, bx);

   gl = elm_genlist_add(win);
   evas_object_smart_callback_add(gl, "selected", _gl_selected, NULL);
   evas_object_smart_callback_add(gl, "clicked,double", _gl_double_clicked, NULL);
   evas_object_smart_callback_add(gl, "longpressed", _gl_longpress, NULL);
   // FIXME: This causes genlist to resize the horiz axis very slowly :(
   // Reenable this and resize the window horizontally, then try to resize it back
   //elm_genlist_mode_set(gl, ELM_LIST_LIMIT);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, gl);
   evas_object_show(gl);

   over = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_color_set(over, 0, 0, 0, 0);
   evas_object_event_callback_add(over, EVAS_CALLBACK_MOUSE_MOVE, _move, gl);
   evas_object_repeat_events_set(over, 1);
   evas_object_show(over);
   evas_object_size_hint_weight_set(over, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, over);

   itc1 = elm_genlist_item_class_new();
   itc1->item_style     = "full";
   itc1->func.text_get = gl_text_get;
   itc1->func.content_get  = gl_content_full_get;
   itc1->func.state_get = gl_state_get;
   itc1->func.del       = NULL;

   bt_50 = elm_button_add(win);
   elm_object_text_set(bt_50, "Go to 50");
   evas_object_show(bt_50);
   elm_box_pack_end(bx, bt_50);

   bt_1500 = elm_button_add(win);
   elm_object_text_set(bt_1500, "Go to 1500");
   evas_object_show(bt_1500);
   elm_box_pack_end(bx, bt_1500);

   for (i = 0; i < 2000; i++)
     {
        gli = elm_genlist_item_append(gl, itc1,
                                      (void *)(long)i/* item data */,
                                      NULL/* parent */,
                                      ELM_GENLIST_ITEM_NONE,
                                      gl_sel/* func */,
                                      (void *)(long)(i * 10)/* func data */);
        if (i == 50)
          evas_object_smart_callback_add(bt_50, "clicked", _bt50_cb, gli);
        else if (i == 1500)
          evas_object_smart_callback_add(bt_1500, "clicked", _bt1500_cb, gli);
     }
   elm_genlist_item_class_free(itc1);

   evas_object_resize(win, 480, 800);
   evas_object_show(win);
}

#endif
