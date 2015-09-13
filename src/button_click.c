#include <pebble.h>
#define ACCEL_STEP_MS 50
static Window *window;
static TextLayer *text_layer;

static BitmapLayer *image_layer;
GBitmap *image;

static BitmapLayer *image2_layer;
GBitmap *image2;
GBitmap *image3;
GBitmap *image4;

bool mode = true;
AccelData accel;
static AppTimer *timer;
int gameClock = 0;

static void makeConfirmed(){
  bitmap_layer_set_bitmap(image2_layer, image4);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
  if(mode){
    bitmap_layer_set_bitmap(image2_layer, image3);
  }else{
     bitmap_layer_set_bitmap(image2_layer, image2);
  }
  mode = !mode;
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  makeConfirmed();
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}


static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void timer_callback(void *data) {
  gameClock++;
  accel_service_peek(&accel);
  int y_grav = accel.y;
 // int x_grav = accel.x;
 //int z_grav = accel.z;
   static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u", y_grav);
  text_layer_set_text(text_layer, body_text);
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}


static void window_load(Window *window) {
 
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 130 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Shake it baby!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
 
  
  //MY CODE BEGIN
  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOWD_ICONE);
  image2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONTACTO);
  image3 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LINKO);
  image4 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHECKMARK);
  image_layer = bitmap_layer_create(bounds);
  image2_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_bitmap(image2_layer, image2);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  bitmap_layer_set_alignment(image2_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(image2_layer));
  
  GRect powah2 = (GRect) { .origin = { 0, -50 }, .size = { 150, 150 } };
  layer_set_frame(bitmap_layer_get_layer(image_layer), powah2);
  
  GRect powah1 = (GRect) { .origin = { 0, 10 }, .size = { 150, 150 } };
  layer_set_frame(bitmap_layer_get_layer(image2_layer), powah1);
  
   accel_data_service_subscribe(0, NULL);
   timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
  
  //MY CODE ENDS
   layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
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

int main(void) {
  init();
  app_event_loop();
  deinit();
}