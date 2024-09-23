/**
 * @example read_rgb.ino
 */

#include "color_sensor_nlcs11.h"

#define INFINITE_LOOP_ON_FAILURE InfiniteLoopOnFailure(__FUNCTION__, __LINE__)

namespace {
emakefun::ColorSensorNlcs11 g_color_sensor;
void InfiniteLoopOnFailure(const char* function, const uint32_t line_number) {
  Serial.println(String(F("entering an infinite loop due to failure in ")) +
                 function + F(", at line number: ") + line_number);
  while (true) {
    yield();
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  Serial.println(F("setup"));

  Wire.begin();

  Serial.println(String(F("color sensor lib version: ")) +
                 emakefun::ColorSensorNlcs11::kVersionMajor + "." +
                 emakefun::ColorSensorNlcs11::kVersionMinor + "." +
                 emakefun::ColorSensorNlcs11::kVersionPatch);

  const auto ret = g_color_sensor.Initialize();

  if (emakefun::ColorSensorNlcs11::kOK == ret) {
    Serial.println(F("color sensor initialization successful"));
  } else {
    Serial.println(String(F("color sensor device initialization failed: ")) +
                   ret);
    INFINITE_LOOP_ON_FAILURE;
  }

  Serial.println(F("setup successful"));
}

void loop() {
  const auto rgb = g_color_sensor.Rgb();
  Serial.println(String("r: ") + rgb.r + ", g: " + rgb.g + ", b: " + rgb.b);
  delay(50);
}
