#include "zhimi_humidifier.h"
#include "esphome/core/log.h"

namespace esphome {
namespace zhimi_humidifier {

static const char *const TAG = "zhimi_humidifier";

static const uint8_t ZHIMI_HUMIDIFIER_STATIC_HEADER[] = {0xAA, 0x93};  // byte 0 and 1 are static

// because this implementation is currently rx-only, there is nothing to setup
void ZhimiWaterSerialComponent::setup() {}

void ZhimiWaterSerialComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "zhimi_humidifier:");

  this->check_uart_settings(1200);
}

void ZhimiWaterSerialComponent::loop() {
  while (this->available() != 0) {
    this->read_byte(&this->data_[this->data_index_]);
    auto check = this->check_byte_();
    if (!check.has_value()) {
      // finished
      this->parse_data_();
      this->data_index_ = 0;
    } else if (!*check) {
      // wrong data
      ESP_LOGV(TAG, "Byte %i of received data frame is invalid.", this->data_index_);
      this->data_index_ = 0;
    } else {
      // next byte
      this->data_index_++;
    }
  }
}

float ZhimiWaterSerialComponent::get_setup_priority() const { return setup_priority::DATA; }

optional<bool> ZhimiWaterSerialComponent::check_byte_() const {
  const uint8_t index = this->data_index_;
  const uint8_t byte  = this->data_[index];
  // header
  if (index < 2)
    return byte == ZHIMI_HUMIDIFIER_STATIC_HEADER[index];
  // payload bytes (water, bucket)
  if (index == 2 || index == 3)
    return true;
  // checksum byte at index 4 → end-of-frame
  if (index == 4)
    return {};  // signal “finished”
  // beyond frame length → invalid
  return false;
}

void ZhimiWaterSerialComponent::parse_data_() {


  const auto water_byte  = this->data_[2];
  const auto bucket_byte = this->data_[3];

  bool has_water = (water_byte == 0x69);
  bool has_bucket = (bucket_byte == 0x3c);


  this->has_water_->publish_state( has_water );
  this->has_bucket_->publish_state( has_bucket );
}

}  // namespace zhimi_humidifier
}  // namespace esphome
