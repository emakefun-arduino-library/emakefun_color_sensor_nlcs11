#include "color_sensor_nlcs11.h"

#include <Arduino.h>

namespace emakefun {
namespace {
constexpr uint16_t kIntegrationTimes[] = {10, 20, 40, 80, 100, 200, 400, 800};
}  // namespace

ColorSensorNlcs11::ColorSensorNlcs11(const Gain gain,
                                     const IntegrationTime integration_time,
                                     const uint8_t i2c_address,
                                     TwoWire& wire)
    : i2c_address_(i2c_address), wire_(wire), gain_(gain), integration_time_(integration_time) {
  // do somethings
}

ColorSensorNlcs11::ErrorCode ColorSensorNlcs11::Initialize() {
  ErrorCode ret = kOK;
  wire_.beginTransmission(i2c_address_);
  wire_.write(0x80);
  wire_.write(0x03);
  wire_.write(gain_ << 4 | integration_time_);
  ret = static_cast<ErrorCode>(wire_.endTransmission());
  return ret;
}

bool ColorSensorNlcs11::GetColor(Color* const color) const {
  if (color == nullptr) {
    return false;
  }

  if (last_read_time_ == 0) {
    last_read_time_ = millis();
    return false;
  }

  if (millis() - last_read_time_ < kIntegrationTimes[integration_time_]) {
    return false;
  }

  last_read_time_ = millis();

  wire_.beginTransmission(i2c_address_);
  wire_.write(0xA0);
  wire_.endTransmission();

  // 请求从传感器读取4个字节的数据
  wire_.requestFrom(i2c_address_, sizeof(*color));

  // 确认读取的数据大小是否正确
  if (wire_.available() == sizeof(*color)) {
    wire_.readBytes(reinterpret_cast<uint8_t*>(color), sizeof(*color));
  } else {
    *color = Color{};
    return false;
  }

  if (color->c == 0) {
    *color = Color{};
    return false;
  }

  color->r = static_cast<uint16_t>((float)color->r / color->c * 255);
  color->g = static_cast<uint16_t>((float)color->g / color->c * 255);
  color->b = static_cast<uint16_t>((float)color->b / color->c * 255);

  return true;
}

}  // namespace emakefun