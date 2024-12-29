#ifndef ESP32_BLE_MOUSE_H
#define ESP32_BLE_MOUSE_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BleConnectionStatus.h"
#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_BACK 8
#define MOUSE_FORWARD 16
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE) #For compatibility with the Mouse library

class BleMouse
{
private:
    uint8_t _buttons;
    BleConnectionStatus *connectionStatus;
    BLEHIDDevice *hid;
    BLECharacteristic *inputMouse;
    BLECharacteristic *featWheelMul;
    void buttons(uint8_t b);
    void rawAction(uint8_t msg[], char msgSize);
    static void taskServer(void *pvParameter);

public:
    BleMouse(std::string deviceName = "Scroller", std::string deviceManufacturer = "Custom", uint8_t batteryLevel = 100);
    void begin(void);
    void end(void);
    void scroll(signed char wheel = 0);
    bool isConnected(void);
    void setBatteryLevel(uint8_t level);
    uint8_t batteryLevel;
    std::string deviceManufacturer;
    std::string deviceName;

protected:
    virtual void onStarted(BLEServer *pServer) {};
};

class wheelFeatCallback : public BLECharacteristicCallbacks
{
public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
    void onRead(BLECharacteristic *pCharacteristic) override;

    wheelFeatCallback();
};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_MOUSE_H