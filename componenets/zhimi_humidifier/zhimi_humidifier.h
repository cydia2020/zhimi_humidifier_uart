#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace zhimi_humidifier {

class ZhimiWaterSerialComponent : public binary_sensor::BinarySensor, public Component, public uart::UARTDevice {
 public:
  ZhimiWaterSerialComponent() = default;

  void set_has_water(binary_sensor::BinarySensor *has_water) { this->has_water_ = has_water; }
  void set_has_bucket(binary_sensor::BinarySensor *has_bucket) { this->has_bucket_ = has_bucket; }

  void setup() override;
  void dump_config() override;
  void loop() override;

  float get_setup_priority() const override;

 protected:
  optional<bool> check_byte_() const;
  void parse_data_();

  uint8_t data_[5];
  uint8_t data_index_{0};
  binary_sensor::BinarySensor *has_water_;
  binary_sensor::BinarySensor *has_bucket_;
};

}  // namespace zhimi_humidifier
}  // namespace esphome
