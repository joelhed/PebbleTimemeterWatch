#include <pebble.h>
#include "main.h"

static Window *s_main_window;

static TextLayer *s_clock_label;
static GFont s_clock_font;

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
  
  // Read Color codes
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

static void setup_face() {
  //graphics_context_set_fill_color(ctx, GColorBlack);
  //graphics_fill_rect(ctx, GRect(bounds.size.w / 2 - 1, bounds.size.h / 2 - 1, 3, 3), 0, GCornerNone);
  
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
  setup_face();

  /*
  // Create GBitmap and add it to Background Layer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND); // Replace image in RESOURCES
  s_background_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Create Text Layer 
  s_clock_label = text_layer_create(GRect(0, 40, bounds.size.w, 40));  
  text_layer_set_background_color(s_clock_label, GColorClear);
  text_layer_set_text_color(s_clock_label, GColorBlack); // Set Clock Color
  text_layer_set_text_alignment(s_clock_label, GTextAlignmentCenter);
  s_clock_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CLOCK_34));
  text_layer_set_font(s_clock_label, s_clock_font);
  layer_add_child(window_layer, text_layer_get_layer(s_clock_label));
  */
  
  // Create Hands Layer and add it to the top of Root Window
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, proc_hands_update);
  layer_add_child(window_layer, s_hands_layer);
  
  
  update_time();
}

static void main_window_unload(Window *window) {  
  /*
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);  
  
  fonts_unload_custom_font(s_clock_font);
  text_layer_destroy(s_clock_label);
  */
  
  
}

static void init(void) {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack); // default background color
    
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload  
  });
  
  window_stack_push(s_main_window, true);
  
  // Setup hands 
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
