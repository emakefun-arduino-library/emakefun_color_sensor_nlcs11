#include "color_sensor_nlcs11.h"

#include <Arduino.h>

namespace emakefun {
namespace {
constexpr uint16_t kMaxRawR = 500;
constexpr uint16_t kMaxRawG = 1100;
constexpr uint16_t kMaxRawB = 800;

float Map(const float value, const float in_min, const float in_max,
          const float out_min, const float out_max) {
  if (value <= in_min) {
    return out_min;
  } else if (value >= in_max) {
    return out_max;
  } else {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
}
}  // namespace

ColorSensorNlcs11::ColorSensorNlcs11(const uint8_t i2c_address, TwoWire &wire)
    : i2c_address_(i2c_address), wire_(wire) {
  // do somethings
}

ColorSensorNlcs11::ErrorCode ColorSensorNlcs11::Initialize() {
  ErrorCode ret = kOK;
  wire_.beginTransmission(i2c_address_);
  wire_.write(0x80);
  wire_.write(0x03);
  ret = static_cast<ErrorCode>(wire_.endTransmission());
  return ret;
}

ColorSensorNlcs11::Color ColorSensorNlcs11::GetColor() const {
  Color color;
  uint16_t value[3] = {0};

  wire_.beginTransmission(i2c_address_);
  wire_.write(0xA0);
  wire_.endTransmission();

  // 请求从传感器读取3个字节的数据
  wire_.requestFrom(i2c_address_, sizeof(value));

  // 确认读取的数据大小是否正确
  if (wire_.available() == sizeof(value)) {
    wire_.readBytes(reinterpret_cast<uint8_t *>(value), sizeof(value));

    color.r = Map(value[0], 0, kMaxRawR, 0, 255);
    color.g = Map(value[1], 0, kMaxRawG, 0, 255);
    color.b = Map(value[2], 0, kMaxRawB, 0, 255);
  }
  return color;
}

}  // namespace emakefun