#include "BleConnectionStatus.h"

#include <FastLED.h>
#if defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
#endif

BleConnectionStatus::BleConnectionStatus(void)
{
}

void BleConnectionStatus::onConnect(BLEServer *pServer)
{
  this->connected = true;
  BLE2902 *desc = (BLE2902 *)this->inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(true);
  ESP_LOGI("####", "Connected");
}

void BleConnectionStatus::onDisconnect(BLEServer *pServer)
{
  this->connected = false;
  BLE2902 *desc = (BLE2902 *)this->inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(false);

  ESP_LOGI("####", "Disconnected -> readvertising...");
  pServer->getAdvertising()->start();
}