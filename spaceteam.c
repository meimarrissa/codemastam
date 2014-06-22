#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static TextLayer *up_layer;
static TextLayer *select_layer;
static TextLayer *down_layer;
static char control[20];
static char up[20];
static char select[20];
static char down[20];
//static char action[7];


static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {

    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
 
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message received by pebble");
    Tuple *control_tuple = dict_find(iter, 0);
    Tuple *up_tuple = dict_find(iter, 1);
    Tuple *select_tuple = dict_find(iter, 2);
    Tuple *down_tuple = dict_find(iter, 3);

    if(control_tuple){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "control wasn't NULL");
      strncpy(control, control_tuple->value->cstring, 20);
      text_layer_set_text(text_layer, control);
    }
    if(up_tuple){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "up wasn't NULL");
      strncpy(up, up_tuple->value->cstring, 20);
      text_layer_set_text(up_layer, up);
    }
     if(select_tuple){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "select wasn't NULL");
      strncpy(select, select_tuple->value->cstring, 20);
      text_layer_set_text(select_layer, select);
    }
     if(down_tuple){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "down wasn't NULL");
      strncpy(down, down_tuple->value->cstring, 20);
      text_layer_set_text(down_layer, down);
    }

}

static void out_sent_handler(DictionaryIterator *iter, void *context){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message sent by pebble");
}
static void app_message_init() {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  app_message_register_outbox_sent(out_sent_handler);
  // Init buffers
  app_message_open(128, 128);
}

static bool send_messages(char * message){
  DictionaryIterator *dict;
  app_message_outbox_begin(&dict);
  Tuplet tuple = TupletCString(1, message);
  dict_write_tuplet(dict, &tuple);
  //dict_write_end(dict);
  app_message_outbox_send();
  return true;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
  char * temp = "select";
  send_messages(temp);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Up");
  char * temp = "up";
  send_messages(temp);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Down");
  char * temp = "down";
  send_messages(temp);
}

// static void long_select_click_handler(ClickRecognizerRef recognizer, void *context){
//   //text_layer_set_text(text_layer, "Long Select");
// }

// static void long_up_click_handler(ClickRecognizerRef recognizer, void *context){
//   //text_layer_set_text(text_layer, "Long Up");
// }

// static void long_down_click_handler(ClickRecognizerRef recognizer, void *context){
//   //text_layer_set_text(text_layer, "Long Down");
// }

// static void multi_select_click_handler(ClickRecognizerRef recognizer, void * context){
//   text_layer_set_text(text_layer, "Multi select");
// }

// static void multi_up_click_handler(ClickRecognizerRef recognizer, void * context){
//   text_layer_set_text(text_layer, "Multi up");
// }

// static void multi_down_click_handler(ClickRecognizerRef recognizer, void * context){
//   text_layer_set_text(text_layer, "Multi Down");
// }

// void accel_tap_handler(AccelAxisType axis, int32_t direction) {
//   // Process tap on ACCEL_AXIS_X, ACCEL_AXIS_Y or ACCEL_AXIS_Z
//   // Direction is 1 or -1
//   text_layer_set_text(text_layer, "tap");
// }

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  // window_long_click_subscribe(BUTTON_ID_SELECT, 0, NULL, long_select_click_handler);
  // window_long_click_subscribe(BUTTON_ID_UP, 0, NULL, long_up_click_handler);
  // window_long_click_subscribe(BUTTON_ID_DOWN, 0, NULL, long_down_click_handler);
  // window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 10, 150, 1, multi_select_click_handler);
  // window_multi_click_subscribe(BUTTON_ID_UP, 2, 10, 150, 1, multi_up_click_handler);
  // window_multi_click_subscribe(BUTTON_ID_DOWN, 2, 10, 150, 1, multi_down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Welcome to spaceteam!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  up_layer = text_layer_create((GRect) {.origin = {0,0}, .size = {bounds.size.w, 20} });
  text_layer_set_text(up_layer, "Up");
  text_layer_set_text_alignment(up_layer, GTextAlignmentRight);
  text_layer_set_background_color(up_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(up_layer));

  select_layer = text_layer_create((GRect) {.origin = {0,60}, .size = {bounds.size.w, 20} });
  text_layer_set_text(select_layer, "select");
  text_layer_set_text_alignment(select_layer, GTextAlignmentRight);
  text_layer_set_background_color(select_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(select_layer));

  down_layer = text_layer_create((GRect) {.origin = {0,130}, .size = {bounds.size.w, 20} });
  text_layer_set_text(down_layer, "down");
  text_layer_set_text_alignment(down_layer, GTextAlignmentRight);
  text_layer_set_background_color(down_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(down_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  app_message_init();
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
 
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

// void handle_init(void) {
//   accel_tap_service_subscribe(&accel_tap_handler);
// }

// void handle_deinit(void) {
//   accel_tap_service_unsubscribe();
// }
int main(void) {
  init();
  //handle_init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  //handle_deinit();
  deinit();
}
