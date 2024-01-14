#include <lvgl.h>
#include <TFT_eSPI.h>
#include "screens.h"
#include "images.h"
#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
  lv_img_set_src(objects.input, &img_connected);
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }

  lv_img_set_src(objects.input, &img_disconnected);
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
    "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
    ctl->index(),        // Controller Index
    ctl->dpad(),         // DPAD
    ctl->buttons(),      // bitmask of pressed buttons
    ctl->axisX(),        // (-511 - 512) left X Axis
    ctl->axisY(),        // (-511 - 512) left Y axis
    ctl->axisRX(),       // (-511 - 512) right X axis
    ctl->axisRY(),       // (-511 - 512) right Y axis
    ctl->brake(),        // (0 - 1023): brake button
    ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
    ctl->miscButtons(),  // bitmak of pressed "misc" buttons
    ctl->gyroX(),        // Gyro X
    ctl->gyroY(),        // Gyro Y
    ctl->gyroZ(),        // Gyro Z
    ctl->accelX(),       // Accelerometer X
    ctl->accelY(),       // Accelerometer Y
    ctl->accelZ()        // Accelerometer Z
  );

  char buffer[256];  // Assicurati che la dimensione del buffer sia sufficiente per la tua stringa

  snprintf(buffer, sizeof(buffer),
           "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
           "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
           ctl->index(),        // Controller Index
           ctl->dpad(),         // DPAD
           ctl->buttons(),      // bitmask of pressed buttons
           ctl->axisX(),        // (-511 - 512) left X Axis
           ctl->axisY(),        // (-511 - 512) left Y axis
           ctl->axisRX(),       // (-511 - 512) right X axis
           ctl->axisRY(),       // (-511 - 512) right Y axis
           ctl->brake(),        // (0 - 1023): brake button
           ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
           ctl->miscButtons(),  // bitmask of pressed "misc" buttons
           ctl->gyroX(),        // Gyro X
           ctl->gyroY(),        // Gyro Y
           ctl->gyroZ(),        // Gyro Z
           ctl->accelX(),       // Accelerometer X
           ctl->accelY(),       // Accelerometer Y
           ctl->accelZ()        // Accelerometer Z
  );

  lv_label_set_text(objects.debug, buffer);
  lv_bar_set_value(objects.battery, ctl->battery(), LV_ANIM_OFF);
}

void processGamepad(ControllerPtr ctl) {
  if (ctl->a()) {
    lv_img_set_src(objects.input, &img_button_a);
  } else if (ctl->b()) {
    lv_img_set_src(objects.input, &img_button_b);
  } else if (ctl->x()) {
    lv_img_set_src(objects.input, &img_button_x);
  } else if (ctl->y()) {
    lv_img_set_src(objects.input, &img_button_y);
  } else if (ctl->l1()) {
    lv_img_set_src(objects.input, &img_bumper_l);
  } else if (ctl->l2()) {
    lv_img_set_src(objects.input, &img_analog_trigger_l);
  } else if (ctl->r1()) {
    lv_img_set_src(objects.input, &img_bumper_r);
  } else if (ctl->r2()) {
    lv_img_set_src(objects.input, &img_analog_trigger_r);
  } else if (ctl->thumbL()) {
    lv_img_set_src(objects.input, &img_analog_l);
  } else if (ctl->thumbR()) {
    lv_img_set_src(objects.input, &img_analog_r);
  } else if (ctl->miscSystem()) {
    lv_img_set_src(objects.input, &img_connected);
  } else if (ctl->miscSelect()) {
    lv_img_set_src(objects.input, &img_button_view);
  } else if (ctl->miscStart()) {
    lv_img_set_src(objects.input, &img_button_menu);
  } else if (ctl->miscCapture()) {
    lv_img_set_src(objects.input, &img_button_start);
  } else if (ctl->dpad() == DPAD_UP) {
    lv_img_set_src(objects.input, &img_dpad_up);
  } else if (ctl->dpad() == DPAD_DOWN) {
    lv_img_set_src(objects.input, &img_dpad_down);
  } else if (ctl->dpad() == DPAD_LEFT) {
    lv_img_set_src(objects.input, &img_dpad_l);
  } else if (ctl->dpad() == DPAD_RIGHT) {
    lv_img_set_src(objects.input, &img_dpad_r);
  }
  dumpGamepad(ctl);
}

// Definizione delle dimensioni dello schermo
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

// Inizializzazione dell'oggetto TFT_eSPI per il display
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

// Inizializzazione del buffer e della struttura per il display LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

// Funzione per il flush del display LVGL su TFT_eSPI
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

void setup() {

  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  // BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "vritual device", is a mouse
  // By default it is disabled.
  BP32.enableVirtualDevice(false);

  // Inizializza il modulo LVGL
  lv_init();

  // Inizializza il display TFT_eSPI
  tft.begin();
  tft.setRotation(1);

  // Inizializza il buffer e la struttura per il display LVGL
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  // Inizializza il driver del display LVGL
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;  // Assegna la funzione di flush
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Carica la schermata iniziale LVGL
  create_screens();
  lv_scr_load(objects.main);
}

void loop() {
  BP32.update();

  // It is safe to always do this before using the gamepad API.
  // This guarantees that the gamepad is valid and connected.
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr myController = myControllers[i];

    if (myController && myController->isConnected()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.printf("Data not available yet\n");
        continue;
      }
      // See ArduinoController.h for all the available functions.
    }
  }
  lv_task_handler();
  delay(150);
}
