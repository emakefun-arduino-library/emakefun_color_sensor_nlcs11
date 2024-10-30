#pragma once

#ifndef EMAKEFUN_COLOR_SENSOR_NLCS11_H_
#define EMAKEFUN_COLOR_SENSOR_NLCS11_H_

#include <Wire.h>
#include <stdint.h>

namespace emakefun {
class ColorSensorNlcs11 {
 public:
  static constexpr uint32_t kVersionMajor = 2;
  static constexpr uint32_t kVersionMinor = 0;
  static constexpr uint32_t kVersionPatch = 2;
  static constexpr uint8_t kDefaultI2cAddress = 0x43;

  /**
   * @enum ErrorCode
   * @brief 错误码
   */
  enum ErrorCode : uint32_t {
    kOK = 0,                                  /**< 0：成功 */
    kI2cDataTooLongToFitInTransmitBuffer = 1, /**< 1：I2C数据太长，无法装入传输缓冲区 */
    kI2cReceivedNackOnTransmitOfAddress = 2,  /**< 2：在I2C发送地址时收到NACK */
    kI2cReceivedNackOnTransmitOfData = 3,     /**< 3：在I2C发送数据时收到NACK */
    kI2cOtherError = 4,                       /**< 4：其他I2C错误 */
    kI2cTimeout = 5,                          /**< 5：I2C通讯超时 */
    kInvalidParameter = 6,                    /**< 6：参数错误 */
    kUnknownError = 7,                        /**< 7：未知错误*/
  };

  enum Gain : uint8_t {
    kGain1X,   /**<  No gain  */
    kGain1p5X, /**<  1.5x gain  */
    kGain2X,   /**<  2x gain */
    kGain2p5X, /**<  2.5x gain */
  };

  enum IntegrationTime : uint8_t {
    kIntegrationTime10ms,
    kIntegrationTime20ms,
    kIntegrationTime40ms,
    kIntegrationTime80ms,
    kIntegrationTime100ms,
    kIntegrationTime200ms,
    kIntegrationTime400ms,
    kIntegrationTime800ms,
  };

  struct Color {
    /* data */
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;
    uint16_t c = 0;
  };

  explicit ColorSensorNlcs11(const Gain gain,
                             const IntegrationTime integration_time,
                             const uint8_t i2c_address = kDefaultI2cAddress,
                             TwoWire& wire = Wire);

  explicit ColorSensorNlcs11(const Gain gain, const IntegrationTime integration_time, TwoWire& wire)
      : ColorSensorNlcs11(gain, integration_time, kDefaultI2cAddress, wire) {
  }

  /**
   * @brief 初始化函数
   * @return 返回值请参考 @ref ErrorCode
   */
  ErrorCode Initialize();

  bool GetColor(Color* const color) const;

 private:
  ColorSensorNlcs11(const ColorSensorNlcs11&) = delete;
  ColorSensorNlcs11& operator=(const ColorSensorNlcs11&) = delete;

  const uint8_t i2c_address_ = kDefaultI2cAddress;
  TwoWire& wire_ = Wire;
  const Gain gain_ = kGain1X;
  const IntegrationTime integration_time_ = kIntegrationTime10ms;
  mutable uint64_t last_read_time_ = 0;
  // mutable Color last_color_;
};
}  // namespace emakefun
#endif