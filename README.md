# scroller
BLE HID Scroll wheel based on ESP32 and AS5600

![PoC](poc.png)

USP:
- Uses the Resolution Multiplier of a HID Mouse thus allowing pixel accurate scrolling if one tinkers long enough with the settings

Components:
- ESP32 (YD-ESP32-S3) - any other should also work as long as it has BLE
- AS5600 - magnetic encoder
- 32mm (outside) bearing
- 3d print files 

Common issues:
- In Windows every time you change the HID Descriptor make sure to remove the paired device via the Bluetooth device screen, not Device Manager as that does not clear the stored Descriptor!


