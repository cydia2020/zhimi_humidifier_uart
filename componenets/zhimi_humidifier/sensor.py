import esphome.codegen as cg
from esphome.components import binary_sensor, uart
import esphome.config_validation as cv
from esphome.const import CONF_ID, DEVICE_CLASS_MOISTURE, DEVICE_CLASS_SAFETY

CODEOWNERS = ["@cydia2020"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["uart"]

zhimi_humidifier_ns = cg.esphome_ns.namespace("zhimi_humidifier")
ZhimiWaterSerialComponent = zhimi_humidifier_ns.class_(
    "ZhimiWaterSerialComponent", cg.Component, uart.UARTDevice
)

CONF_HAS_WATER = "has_water"
CONF_HAS_BUCKET = "has_bucket"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ZhimiWaterSerialComponent),
    cv.Optional(CONF_HAS_WATER): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_MOISTURE
    ),
    cv.Optional(CONF_HAS_BUCKET): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_SAFETY
    ),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_HAS_WATER in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_HAS_WATER])
        cg.add(var.set_has_water(sens))
    if CONF_HAS_BUCKET in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_HAS_BUCKET])
        cg.add(var.set_has_bucket(sens))