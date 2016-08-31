#include <pebble.h>
#include "main.h"

static Window *s_main_window;

static TextLayer *s_clock_label;
static GFont s_clock_font;

static GFont s_month_font;
TextLayer *month_label[12];

static GFont s_hour_font;
TextLayer *hour_label[24];

static GBitmap *s_background_bitmap;
static BitmapLayer *s_background_layer;

static Layer *s_bg_layer, *s_hands_layer;
static GPath *s_hour_arrow, *s_date_arrow;

static int32_t colorcode_background, colorcode_hourhand, colorcode_datehand;

static void setup_colors() {
  colorcode_background = persist_read_int(KEY_COLOR_BACKGROUND) ? persist_read_int(KEY_COLOR_BACKGROUND) : COLOR_DEFAULT_BACKGROUND;
  colorcode_hourhand = persist_read_int(KEY_COLOR_HOURHAND) ? persist_read_int(KEY_COLOR_HOURHAND) : COLOR_DEFAULT_HOURHAND;
  colorcode_datehand = persist_read_int(KEY_COLOR_DATEHAND) ? persist_read_int(KEY_COLOR_DATEHAND) : COLOR_DEFAULT_DATEHAND;
}

static void proc_hands_update (Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  // GPoint center = grect_center_point(&bounds);
  graphics_context_set_antialiased(ctx, true);

  setup_colors();
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);  

  // hour hand in 24 hours
  GColor color_hourhand = GColorFromHEX(colorcode_hourhand);
  graphics_context_set_fill_color(ctx, color_hourhand);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * ((t->tm_hour * 6) + (t->tm_min / 10))) / (24 * 6));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
  
  // date hand in 365 days
  GColor color_datehand = GColorFromHEX(colorcode_datehand);
  graphics_context_set_fill_color(ctx, color_datehand);
  graphics_context_set_stroke_color(ctx, GColorBlack);  
  int32_t dates_of_year = 365; // todo
  gpath_rotate_to(s_date_arrow, TRIG_MAX_ANGLE * t->tm_yday / dates_of_year);
  gpath_draw_filled(ctx, s_date_arrow);
  gpath_draw_outline(ctx, s_date_arrow);
  
  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(bounds.size.w / 2 - 1, bounds.size.h / 2 - 1, 3, 3), 0, GCornerNone);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) { 
  }
}

static void update_time() {
  
}

static void setup_face_month() {
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  
  s_month_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_YEARHAND_14));
  int32_t label_radius = PBL_IF_ROUND_ELSE(35, 30);
  int32_t label_size = 22;
  //const char *mn[] = {"1","2","3","4","5","6","7","8","9","10","11","12"};
  const char *mn[] = {"J","F","M","A","M","J","J","A","S","O","N","D"};
 
  for(int32_t m=0;m<12;++m) {
    int32_t angle = TRIG_MAX_ANGLE * (m+6) * 5 / 60;
    GPoint pos;
    pos.x = (-sin_lookup(angle) * label_radius / TRIG_MAX_RATIO) + center.x - (label_size/2);
    pos.y = (cos_lookup(angle) * label_radius / TRIG_MAX_RATIO) + center.y - (label_size/2);
    month_label[m] = text_layer_create(GRect(pos.x, pos.y, label_size, label_size));
    text_layer_set_background_color(month_label[m], GColorClear);
    text_layer_set_text_color(month_label[m], GColorLightGray);
    text_layer_set_text_alignment(month_label[m], GTextAlignmentCenter);  
    text_layer_set_font(month_label[m], s_month_font);
    text_layer_set_text(month_label[m], mn[m]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "adding month: %s at %d, %d angle (%d / %d) from %d, %d",mn[m], pos.x, pos.y, (int)angle, TRIG_MAX_ANGLE, center.x, center.y);
    layer_add_child(window_layer, text_layer_get_layer(month_label[m]));
  }
  
}

static void setup_face_hour() {
Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_YEARHAND_14));
  int32_t label_radius = PBL_IF_ROUND_ELSE(80, 66);
  int32_t label_size = 18;
  //const char *num[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23"};
  const char *num[] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"};
 
  for(int32_t m=0;m<24;++m) {
    int32_t angle = TRIG_MAX_ANGLE * (m+12) * 15 / 360;
    GPoint pos;
    pos.x = (-sin_lookup(angle) * label_radius / TRIG_MAX_RATIO) + center.x - (label_size/2);
    pos.y = (cos_lookup(angle) * label_radius / TRIG_MAX_RATIO) + center.y - (label_size/2);
    hour_label[m] = text_layer_create(GRect(pos.x, pos.y, label_size, label_size));
    text_layer_set_background_color(hour_label[m], GColorClear);
    text_layer_set_text_color(hour_label[m], GColorWhite);
    text_layer_set_text_alignment(hour_label[m], GTextAlignmentCenter);  
    text_layer_set_font(hour_label[m], s_hour_font);
    text_layer_set_text(hour_label[m], num[m]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "adding hour: %s at %d, %d angle (%d / %d) from %d, %d",num[m], pos.x, pos.y, (int)angle, TRIG_MAX_ANGLE, center.x, center.y);
    layer_add_child(window_layer, text_layer_get_layer(hour_label[m]));
  }
}

static void setup_hands() {
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  s_date_arrow = gpath_create(&DATE_HAND_POINTS);
  
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_hour_arrow, center);
  gpath_move_to(s_date_arrow, center);

}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Setup face
  setup_face_month();
  setup_face_hour();

  // Create Hands Layer and add it to the top of Root Window
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, proc_hands_update);
  layer_add_child(window_layer, s_hands_layer);
  
  
  update_time();
}

static void main_window_unload(Window *window) {  
  fonts_unload_custom_font(s_month_font);
  fonts_unload_custom_font(s_hour_font);

  for(int32_t i=0; i<12; ++i) {
    text_layer_destroy(month_label[i]);
  }
  for(int32_t i=0; i<24; ++i) {
    text_layer_destroy(hour_label[i]);
  }
  
} 

static void init(void) {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack); // default background color
    
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload  
  });
  
  window_stack_push(s_main_window, true);
  
  setup_hands();
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

static void deinit(void) {
  gpath_destroy(s_hour_arrow);
  gpath_destroy(s_date_arrow);
  
  tick_timer_service_unsubscribe();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
