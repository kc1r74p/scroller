#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
#include <driver/adc.h>
#include "sdkconfig.h"

#include "BleConnectionStatus.h"
#include "BleMouse.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
#endif

const uint8_t REPORT_ID_MOUSE = 1;
const uint8_t REPORT_ID_MULTIPLIER = 2;

const uint8_t _hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01, // USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,      // USAGE (Mouse)
    COLLECTION(1), 0x01, // COLLECTION (Application)
    USAGE_PAGE(1), 0x01, //     USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,      //     USAGE (Mouse)
    COLLECTION(1), 0x02, //     COLLECTION (Logical)
    REPORT_ID(1), REPORT_ID_MOUSE,  //   Report ID (REPORT_ID_MOUSE)
    USAGE(1), 0x01,                 //   USAGE (Pointer)
    COLLECTION(1), 0x00,            //   COLLECTION (Physical)
    USAGE_PAGE(1), 0x09,            //      USAGE_PAGE (Button)
    USAGE_MINIMUM(1), 0x01,         //      USAGE_MINIMUM (Button 1)
    USAGE_MAXIMUM(1), 0x08,         //      USAGE_MAXIMUM (Button 8)
    REPORT_COUNT(1), 0x08,          //      REPORT_COUNT (8)
    REPORT_SIZE(1), 0x01,           //      REPORT SIZE (1)
    LOGICAL_MAXIMUM(1), 0x01,       //      LOGICAL_MAXIMUM (1)
    HIDINPUT(1), 0x02,              //      INPUT (Data, Variable, Absolute)
    USAGE_PAGE(1), 0x01,            //      USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x30,                 //      USAGE (X)
    USAGE(1), 0x31,                 //      USAGE (Y)
    REPORT_COUNT(1), 0x02,          //      REPORT_COUNT (2)
    REPORT_SIZE(1), 0x08,           //      REPORT SIZE (8)
    LOGICAL_MINIMUM(1), 0x81,       //      LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f,       //      LOGICAL_MAXIMUM (127)
    HIDINPUT(1), 0x06,              //      INPUT (Data, Variable, Relative)
    COLLECTION(1), 0x02,            //      COLLECTION (Logical)
    REPORT_ID(1), REPORT_ID_MULTIPLIER, //      Report ID (REPORT_ID_MULTIPLIER)
    USAGE(1), 0x48,                     //      Usage (Resolution Multiplier)
    REPORT_COUNT(1), 0x01,              //      REPORT_COUNT (1)
    REPORT_SIZE(1), 0x02,               //      REPORT SIZE (2)
    LOGICAL_MINIMUM(1), 0x00,           //      LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01,           //      LOGICAL_MAXIMUM (1)
    PHYSICAL_MINIMUM(1), 0x01,          //      PHYSICAL_MINIMUM (1)
    PHYSICAL_MAXIMUM(1), 0x78,          //      PHYSICAL_MAXIMUM (120)
    FEATURE(1), 0x02,                   //      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    REPORT_ID(1), REPORT_ID_MOUSE,      //      Report ID (REPORT_ID_MOUSE)
    USAGE(1), 0x38,                     //      Usage (Wheel)
    PHYSICAL_MINIMUM(1), 0x00,          //      PHYSICAL_MINIMUM (0)
    PHYSICAL_MAXIMUM(1), 0x00,          //      PHYSICAL_MAXIMUM (0)
    LOGICAL_MINIMUM(1), 0x81,           //      LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f,           //      LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,               //      REPORT_SIZE (8)
    HIDINPUT(1), 0x06,                  //      INPUT (Data, Variable, Relative)
    END_COLLECTION(0),              //      END_COLLECTION
    COLLECTION(1), 0x02,            //      COLLECTION (Logical)
    REPORT_ID(1), REPORT_ID_MULTIPLIER, //      Report ID (REPORT_ID_MULTIPLIER)
    USAGE(1), 0x48,                     //      Usage (Resolution Multiplier)
    REPORT_SIZE(1), 0x02,               //      REPORT SIZE (2)
    LOGICAL_MINIMUM(1), 0x00,           //      LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01,           //      LOGICAL_MAXIMUM (1)
    PHYSICAL_MINIMUM(1), 0x01,          //      PHYSICAL_MINIMUM (1)
    PHYSICAL_MAXIMUM(1), 0x78,          //      PHYSICAL_MAXIMUM (120)
    FEATURE(1), 0x02,                   //      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    PHYSICAL_MINIMUM(1), 0x00,          //      PHYSICAL_MINIMUM (0)
    PHYSICAL_MAXIMUM(1), 0x00,          //      PHYSICAL_MAXIMUM (0)
    REPORT_SIZE(1), 0x04,               //      REPORT_SIZE (4)
    FEATURE(1), 0x03,                   //      Feature
    REPORT_ID(1), REPORT_ID_MOUSE,      //      Report ID (REPORT_ID_MOUSE)
    USAGE_PAGE(1), 0x0C,                //      USAGE_PAGE (Consumer)
    LOGICAL_MINIMUM(1), 0x81,           //      LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f,           //      LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,               //      REPORT_SIZE (8)
    USAGE(2), 0x38, 0x02,               //      USAGE (AC Pan)
    HIDINPUT(1), 0x06,                  //      INPUT (Data, Variable, Relative)
    END_COLLECTION(0),              //      END_COLLECTION
    END_COLLECTION(0),         //   END_COLLECTION
    END_COLLECTION(0),     //   END_COLLECTION
    END_COLLECTION(0)   //  END_COLLECTION
};

BleMouse::BleMouse(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) : _buttons(0), hid(0)
{
    this->deviceName = deviceName;
    this->deviceManufacturer = deviceManufacturer;
    this->batteryLevel = batteryLevel;
    this->connectionStatus = new BleConnectionStatus();
}

void BleMouse::begin(void)
{
    BLEDevice::init(this->deviceName);
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(this->connectionStatus);

    this->hid = new BLEHIDDevice(pServer);
    this->inputMouse = this->hid->inputReport(REPORT_ID_MOUSE);
    this->featWheelMul = this->hid->featureReport(REPORT_ID_MULTIPLIER);
    this->featWheelMul->setCallbacks(new wheelFeatCallback());

    this->connectionStatus->inputMouse = this->inputMouse;

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    ESP_LOGI("####", "report len: %d", sizeof(_hidReportDescriptor));
    this->hid->reportMap((uint8_t *)_hidReportDescriptor, sizeof(_hidReportDescriptor));

    this->hid->manufacturer()->setValue(this->deviceManufacturer);
    this->hid->pnp(0x02, 0x1915, 0xEEF1, 0x0100);
    this->hid->hidInfo(0x00, 0x02);

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->setScanResponse(false);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());

    this->onStarted(pServer);

    this->hid->startServices();
    this->hid->setBatteryLevel(this->batteryLevel);
    pAdvertising->start();
}

void BleMouse::scroll(signed char wheel)
{
    if (this->isConnected())
    {
        int i = 0;
        uint8_t m[5];
        m[i++] = 0x0;
        m[i++] = 0x0;
        m[i++] = 0x0;
        m[i++] = wheel;
        m[i++] = 0x00;
        this->inputMouse->setValue(m, sizeof(m));
        this->inputMouse->notify();
    }
}

bool BleMouse::isConnected(void)
{
    return this->connectionStatus->connected;
}

void BleMouse::setBatteryLevel(uint8_t level)
{
    this->batteryLevel = level;
    if (hid != 0)
        this->hid->setBatteryLevel(this->batteryLevel);
}

void wheelFeatCallback::onRead(BLECharacteristic *pCharacteristic)
{
    std::string buff = pCharacteristic->getValue();
    ESP_LOGI("####", "onRead Feature report data: %d", buff.length());

    for (int i = buff.length() - 1; i >= 0; i--)
    {
        ESP_LOGI("####", "0x%02X", buff[i]);
    }

    if (buff.length() == 1)
    {
        ESP_LOGI("####", "setting multipliers");
        uint8_t m[1];
        m[0] = 0x0F;
        pCharacteristic->setValue(m, sizeof(m));
        pCharacteristic->notify();
    }
}
