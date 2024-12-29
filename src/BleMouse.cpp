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

// org
/*
static const uint8_t _hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01, // USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,      // USAGE (Mouse)
    COLLECTION(1), 0x01, // COLLECTION (Application)
    USAGE(1), 0x01,      //   USAGE (Pointer)
    COLLECTION(1), 0x00, //   COLLECTION (Physical)
    // ------------------------------------------------- Buttons (Left, Right, Middle, Back, Forward)
    USAGE_PAGE(1), 0x09,      //     USAGE_PAGE (Button)
    USAGE_MINIMUM(1), 0x01,   //     USAGE_MINIMUM (Button 1)
    USAGE_MAXIMUM(1), 0x05,   //     USAGE_MAXIMUM (Button 5)
    LOGICAL_MINIMUM(1), 0x00, //     LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01, //     LOGICAL_MAXIMUM (1)
    REPORT_SIZE(1), 0x01,     //     REPORT_SIZE (1)
    REPORT_COUNT(1), 0x05,    //     REPORT_COUNT (5)
    HIDINPUT(1), 0x02,        //     INPUT (Data, Variable, Absolute) ;5 button bits
    // ------------------------------------------------- Padding
    REPORT_SIZE(1), 0x03,  //     REPORT_SIZE (3)
    REPORT_COUNT(1), 0x01, //     REPORT_COUNT (1)
    HIDINPUT(1), 0x03,     //     INPUT (Constant, Variable, Absolute) ;3 bit padding
    // ------------------------------------------------- X/Y position, Wheel
    USAGE_PAGE(1), 0x01,      //     USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x30,           //     USAGE (X)
    USAGE(1), 0x31,           //     USAGE (Y)
    USAGE(1), 0x38,           //     USAGE (Wheel)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,     //     REPORT_SIZE (8)
    REPORT_COUNT(1), 0x03,    //     REPORT_COUNT (3)
    HIDINPUT(1), 0x06,        //     INPUT (Data, Variable, Relative) ;3 bytes (X,Y,Wheel)
    // ------------------------------------------------- Horizontal wheel
    USAGE_PAGE(1), 0x0c,      //     USAGE PAGE (Consumer Devices)
    USAGE(2), 0x38, 0x02,     //     USAGE (AC Pan)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,     //     REPORT_SIZE (8)
    REPORT_COUNT(1), 0x01,    //     REPORT_COUNT (1)
    HIDINPUT(1), 0x06,        //     INPUT (Data, Var, Rel)
    END_COLLECTION(0),        //   END_COLLECTION
    END_COLLECTION(0)         // END_COLLECTION
};
*/

/*
static const uint8_t _hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01, // USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,      // USAGE (Mouse)
    COLLECTION(1), 0x01, // COLLECTION (Application)
    USAGE(1), 0x01,      //   USAGE (Pointer)
    COLLECTION(1), 0x00, //   COLLECTION (Physical)
    // ------------------------------------------------- Buttons (Left, Right, Middle, Back, Forward)
    REPORT_ID(1), 0x1,                 //      Report ID (1)
    USAGE_PAGE(1), 0x09,      //     USAGE_PAGE (Button)
    USAGE_MINIMUM(1), 0x01,   //     USAGE_MINIMUM (Button 1)
    USAGE_MAXIMUM(1), 0x05,   //     USAGE_MAXIMUM (Button 5)
    LOGICAL_MINIMUM(1), 0x00, //     LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01, //     LOGICAL_MAXIMUM (1)
    REPORT_SIZE(1), 0x01,     //     REPORT_SIZE (1)
    REPORT_COUNT(1), 0x05,    //     REPORT_COUNT (5)
    HIDINPUT(1), 0x02,        //     INPUT (Data, Variable, Absolute) ;5 button bits
    // ------------------------------------------------- Padding
    REPORT_SIZE(1), 0x03,  //     REPORT_SIZE (3)
    REPORT_COUNT(1), 0x01, //     REPORT_COUNT (1)
    HIDINPUT(1), 0x03,     //     INPUT (Constant, Variable, Absolute) ;3 bit padding
    // ------------------------------------------------- X/Y position, Wheel
    USAGE_PAGE(1), 0x01,            //      USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x30,                 //      USAGE (X)
    USAGE(1), 0x31,                 //      USAGE (Y)
    REPORT_COUNT(1), 0x02,          //      REPORT_COUNT (2)
    REPORT_SIZE(1), 0x08,           //      REPORT_SIZE (8)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    HIDINPUT(1), 0x06,              //      INPUT (Constant, Variable, Absolute) ;3 bit padding
    COLLECTION(1), 0x02,            //      COLLECTION (Logical)
    REPORT_ID(1), 0x02,                 //      Report ID (18)
    USAGE(1), 0x48,                     //      USAGE (Resolution Multiplier)
    REPORT_COUNT(1), 0x01,              //      REPORT_COUNT (1)
    REPORT_SIZE(1), 0x02,               //      REPORT_SIZE (2)
    LOGICAL_MINIMUM(1), 0x00,           //      LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x0F,           //      LOGICAL_MAXIMUM (1)
    PHYSICAL_MINIMUM(1), 0x01,          //
    PHYSICAL_MAXIMUM(1), 0x0c,          //
    FEATURE(1), 0x02,                   //
    REPORT_ID(1), 0x03,                 //      Report ID (26)
    USAGE(1), 0x38,                     //      USAGE (Wheel)
    PHYSICAL_MINIMUM(1), 0x00,          //      PHYSICAL_MINIMUM (0)
    PHYSICAL_MAXIMUM(1), 0x00,          //      PHYSICAL_MAXIMUM (0)
    REPORT_COUNT(1), 0x01,              //      REPORT_COUNT (1)
    REPORT_SIZE(1), 0x10,               //      REPORT_SIZE (16)
    LOGICAL_MINIMUM(2), 0x01, 0x80,     //      LOGICAL_MINIMUM (-32767)
    LOGICAL_MAXIMUM(2), 0xff, 0x7f,     //      LOGICAL_MAXIMUM (32767)
    HIDINPUT(1), 0x06,                  //      INPUT (Data, Variable, Relative) ;3 bytes (X,Y,Wheel)
    END_COLLECTION(0),        //   END_COLLECTION
    // ------------------------------------------------- Horizontal wheel
    USAGE_PAGE(1), 0x0c,      //     USAGE PAGE (Consumer Devices)
    USAGE(2), 0x38, 0x02,     //     USAGE (AC Pan)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,     //     REPORT_SIZE (8)
    REPORT_COUNT(1), 0x01,    //     REPORT_COUNT (1)
    HIDINPUT(1), 0x06,        //     INPUT (Data, Var, Rel)
    END_COLLECTION(0),        //   END_COLLECTION
    END_COLLECTION(0)         // END_COLLECTION
};
*/

/*
static const uint8_t _hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01, // USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,      // USAGE (Mouse)
    COLLECTION(1), 0x01, // COLLECTION (Application)
    USAGE(1), 0x01,      //   USAGE (Pointer)
    COLLECTION(1), 0x00, //   COLLECTION (Physical)
    // ------------------------------------------------- Buttons (Left, Right, Middle, Back, Forward)
    REPORT_ID(1), 0x01,       //     Report ID (1)
    USAGE_PAGE(1), 0x09,      //     USAGE_PAGE (Button)
    USAGE_MINIMUM(1), 0x01,   //     USAGE_MINIMUM (Button 1)
    USAGE_MAXIMUM(1), 0x05,   //     USAGE_MAXIMUM (Button 5)
    LOGICAL_MINIMUM(1), 0x00, //     LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x01, //     LOGICAL_MAXIMUM (1)
    REPORT_SIZE(1), 0x01,     //     REPORT_SIZE (1)
    REPORT_COUNT(1), 0x03,    //     REPORT_COUNT (3)
    HIDINPUT(1), 0x01,        //     INPUT (Data, Variable, Absolute)
    // ------------------------------------------------- X/Y position, Wheel
    USAGE_PAGE(1), 0x01,            //      USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x30,                 //      USAGE (X)
    USAGE(1), 0x31,                 //      USAGE (Y)
    REPORT_COUNT(1), 0x02,          //      REPORT_COUNT (2)
    REPORT_SIZE(1), 0x08,           //      REPORT_SIZE (8)
    LOGICAL_MINIMUM(1), 0x81,       //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f,       //     LOGICAL_MAXIMUM (127)
    HIDINPUT(1), 0x00,              //      INPUT (Constant, Variable, Absolute)
    COLLECTION(1), 0x02,            //      COLLECTION (Logical)
    REPORT_ID(1), 0x02,                 //      Report ID (2)
    USAGE(1), 0x48,                     //      USAGE (Resolution Multiplier)
    REPORT_COUNT(1), 0x01,              //      REPORT_COUNT (1)
    REPORT_SIZE(1), 0x02,               //      REPORT_SIZE (2)
    LOGICAL_MINIMUM(1), 0x00,           //      LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x0F,           //      LOGICAL_MAXIMUM (1)
    PHYSICAL_MINIMUM(1), 0x01,          //
    PHYSICAL_MAXIMUM(1), 0x0c,          //
    FEATURE(1), 0x02,                   //
    REPORT_ID(1), 0x01,                 //      Report ID (26)
    USAGE(1), 0x38,                     //      USAGE (Wheel)
    PHYSICAL_MINIMUM(1), 0x00,          //      PHYSICAL_MINIMUM (0)
    PHYSICAL_MAXIMUM(1), 0x00,          //      PHYSICAL_MAXIMUM (0)
    REPORT_COUNT(1), 0x01,              //      REPORT_COUNT (1)
    REPORT_SIZE(1), 0x10,               //      REPORT_SIZE (16)
    LOGICAL_MINIMUM(2), 0x01, 0x80,     //      LOGICAL_MINIMUM (-32767)
    LOGICAL_MAXIMUM(2), 0xff, 0x7f,     //      LOGICAL_MAXIMUM (32767)
    HIDINPUT(1), 0x06,                  //      INPUT (Data, Variable, Relative) ;3 bytes (X,Y,Wheel)
    END_COLLECTION(0),        //   END_COLLECTION
    END_COLLECTION(0),        //   END_COLLECTION
    END_COLLECTION(0)         // END_COLLECTION
};
*/

/*
static const uint8_t _hidReportDescriptor[] = {
     0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
     0x09, 0x02,        // USAGE (Mouse)
     0xa1, 0x01,        // COLLECTION (Application)
     0x09, 0x02,        //   USAGE (Mouse)
     0xa1, 0x02,        //   COLLECTION (Logical)
     0x09, 0x01,        //     USAGE (Pointer)
     0xa1, 0x00,        //     COLLECTION (Physical)
                        // ------------------------------  Buttons
     0x05, 0x09,        //       USAGE_PAGE (Button)
     0x19, 0x01,        //       USAGE_MINIMUM (Button 1)
     0x29, 0x05,        //       USAGE_MAXIMUM (Button 5)
     0x15, 0x00,        //       LOGICAL_MINIMUM (0)
     0x25, 0x01,        //       LOGICAL_MAXIMUM (1)
     0x75, 0x01,        //       REPORT_SIZE (1)
     0x95, 0x05,        //       REPORT_COUNT (5)
     0x81, 0x02,        //       INPUT (Data,Var,Abs)
                        // ------------------------------  Padding
     0x75, 0x03,        //       REPORT_SIZE (3)
     0x95, 0x01,        //       REPORT_COUNT (1)
     0x81, 0x03,        //       INPUT (Cnst,Var,Abs)
                        // ------------------------------  X,Y position
     0x05, 0x01,        //       USAGE_PAGE (Generic Desktop)
     0x09, 0x30,        //       USAGE (X)
     0x09, 0x31,        //       USAGE (Y)
     0x15, 0x81,        //       LOGICAL_MINIMUM (-127)
     0x25, 0x7f,        //       LOGICAL_MAXIMUM (127)
     0x75, 0x08,        //       REPORT_SIZE (8)
     0x95, 0x02,        //       REPORT_COUNT (2)
     0x81, 0x06,        //       INPUT (Data,Var,Rel)
     0xa1, 0x02,        //       COLLECTION (Logical)
                        // ------------------------------  Vertical wheel res multiplier
     0x09, 0x48,        //         USAGE (Resolution Multiplier)
     0x15, 0x00,        //         LOGICAL_MINIMUM (0)
     0x25, 0x0f,        //         LOGICAL_MAXIMUM (1)
     0x35, 0x01,        //         PHYSICAL_MINIMUM (1)
     0x45, 0x0f,        //         PHYSICAL_MAXIMUM (4)
     0x75, 0x02,        //         REPORT_SIZE (2)
     0x95, 0x01,        //         REPORT_COUNT (1)
     0xa4,              //         PUSH
     0xb1, 0x02,        //         FEATURE (Data,Var,Abs)
                        // ------------------------------  Vertical wheel
     0x09, 0x38,        //         USAGE (Wheel)
     0x15, 0x81,        //         LOGICAL_MINIMUM (-127)
     0x25, 0x7f,        //         LOGICAL_MAXIMUM (127)
     0x35, 0x00,        //         PHYSICAL_MINIMUM (0)        - reset physical
     0x45, 0x00,        //         PHYSICAL_MAXIMUM (0)
     0x75, 0x08,        //         REPORT_SIZE (8)
     0x81, 0x06,        //         INPUT (Data,Var,Rel)
     0xc0,              //       END_COLLECTION
     0xa1, 0x02,        //       COLLECTION (Logical)
                        // ------------------------------  Horizontal wheel res multiplier
     0x09, 0x48,        //         USAGE (Resolution Multiplier)
     0xb4,              //         POP
     0xb1, 0x02,        //         FEATURE (Data,Var,Abs)
                        // ------------------------------  Padding for Feature report
     0x35, 0x00,        //         PHYSICAL_MINIMUM (0)        - reset physical
     0x45, 0x00,        //         PHYSICAL_MAXIMUM (0)
     0x75, 0x04,        //         REPORT_SIZE (4)
     0xb1, 0x03,        //         FEATURE (Cnst,Var,Abs)
                        // ------------------------------  Horizontal wheel
     0x05, 0x0c,        //         USAGE_PAGE (Consumer Devices)
     0x0a, 0x38, 0x02,  //         USAGE (AC Pan)
     0x15, 0x81,        //         LOGICAL_MINIMUM (-127)
     0x25, 0x7f,        //         LOGICAL_MAXIMUM (127)
     0x75, 0x08,        //         REPORT_SIZE (8)
     0x81, 0x06,        //         INPUT (Data,Var,Rel)
     0xc0,              //       END_COLLECTION
     0xc0,              //     END_COLLECTION
     0xc0,              //   END_COLLECTION
     0xc0               // END_COLLECTION
};
*/
/*
static const uint8_t _hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01, // USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x02,      // USAGE (Mouse)
    COLLECTION(1), 0x01, // COLLECTION (Application)
    USAGE(1), 0x01,      //   USAGE (Pointer)
    COLLECTION(1), 0x00, //   COLLECTION (Physical)
    REPORT_ID(1), 0x01,  //
    // ------------------------------------------------- Buttons (Left, Right, Middle, Back, Forward)
    USAGE_PAGE(1), 0x09,      //     USAGE_PAGE (Button)
    USAGE_MINIMUM(1), 0x01,   //     USAGE_MINIMUM (Button 1)
    USAGE_MAXIMUM(1), 0x05,   //     USAGE_MAXIMUM (Button 5)
    REPORT_COUNT(1), 0x05,    //     REPORT_COUNT (5)
    REPORT_SIZE(1), 0x01,     //     REPORT_SIZE (1)
    LOGICAL_MAXIMUM(1), 0x01, //     LOGICAL_MAXIMUM (1)
    HIDINPUT(1), 0x02,        //     INPUT (Data, Variable, Absolute) ;5 button bits
    // ------------------------------------------------- Padding
    REPORT_SIZE(1), 0x03,  //     REPORT_SIZE (3)
    REPORT_COUNT(1), 0x01, //     REPORT_COUNT (1)
    HIDINPUT(1), 0x03,     //     INPUT (Constant, Variable, Absolute) ;3 bit padding
    // ------------------------------------------------- X/Y position,
    USAGE_PAGE(1), 0x01,      //     USAGE_PAGE (Generic Desktop)
    USAGE(1), 0x30,           //     USAGE (X)
    USAGE(1), 0x31,           //     USAGE (Y)
    REPORT_COUNT(1), 0x02,    //     REPORT_COUNT (2)
    REPORT_SIZE(1), 0x08,     //     REPORT_SIZE (8)
    LOGICAL_MINIMUM(1), 0x81, //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f, //     LOGICAL_MAXIMUM (127)
    HIDINPUT(1), 0x06,        //     INPUT (Data, Variable, Relative) ;2 bytes (X,Y)
    // ------------------ Wheel
    COLLECTION(1), 0x02,       // COLLECTION (Logical)
    REPORT_ID(1), 0x02,        //
    USAGE(1), 0x48,            //     USAGE (Res Mul)
    REPORT_COUNT(1), 0x01,     //     REPORT_COUNT (1)
    REPORT_SIZE(1), 0x02,      //     REPORT_SIZE (2)
    LOGICAL_MINIMUM(1), 0x00,  //     LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x0F,  //     LOGICAL_MAXIMUM (15)
    PHYSICAL_MINIMUM(1), 0x01, //      PHYSICAL_MINIMUM (1)
    PHYSICAL_MAXIMUM(1), 0x04, //      PHYSICAL_MAXIMUM (4)
    FEATURE(1), 0x02,          //
    //
    REPORT_ID(1), 0x01,        //
    USAGE(1), 0x38,            //     USAGE (Wheel)
    PHYSICAL_MINIMUM(1), 0x00, //      PHYSICAL_MINIMUM (0)
    PHYSICAL_MAXIMUM(1), 0x00, //      PHYSICAL_MAXIMUM (0)
    LOGICAL_MINIMUM(1), 0x81,  //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0xFF,  //     LOGICAL_MAXIMUM (127) 0x7f
    REPORT_SIZE(1), 0x08,      //     REPORT_SIZE (8)
    HIDINPUT(1), 0x06,         //     INPUT (Data, Variable, Relative) ;2 bytes (X,Y)
    END_COLLECTION(0),         // END_COLLECTION
    // ------------------------------------------------- Horizontal wheel
    COLLECTION(1), 0x02,       // COLLECTION (Logical)
    REPORT_ID(1), 0x02,        //
    USAGE(1), 0x48,            //     USAGE (Res Mul)
    REPORT_SIZE(1), 0x02,      //     REPORT_SIZE (2)
    LOGICAL_MINIMUM(1), 0x00,  //     LOGICAL_MINIMUM (0)
    LOGICAL_MAXIMUM(1), 0x0F,  //     LOGICAL_MAXIMUM (15)
    PHYSICAL_MINIMUM(1), 0x01, //      PHYSICAL_MINIMUM (1)
    PHYSICAL_MAXIMUM(1), 0x04, //      PHYSICAL_MAXIMUM (4)
    FEATURE(1), 0x02,          //
    PHYSICAL_MINIMUM(1), 0x00, //      PHYSICAL_MINIMUM (0)
    PHYSICAL_MAXIMUM(1), 0x00, //      PHYSICAL_MAXIMUM (0)
    REPORT_SIZE(1), 0x04,      //     REPORT_SIZE (4)
    FEATURE(1), 0x03,          // const
    REPORT_ID(1), 0x01,        //
    USAGE_PAGE(1), 0x0c,       // USAGE_PAGE (consumer dev)
    LOGICAL_MINIMUM(1), 0x81,  //     LOGICAL_MINIMUM (-127)
    LOGICAL_MAXIMUM(1), 0x7f,  //     LOGICAL_MAXIMUM (127)
    REPORT_SIZE(1), 0x08,      //     REPORT_SIZE (8)
    USAGE(2), 0x38, 0x02,      //     USAGE (AC Pan)
    HIDINPUT(1), 0x06,         //     INPUT (Data, Variable, Relative) ;2 bytes (X,Y)
    END_COLLECTION(0),         // END_COLLECTION
    END_COLLECTION(0),         //   END_COLLECTION
    END_COLLECTION(0)          // END_COLLECTION

};*/

/*
const unsigned char _hidReportDescriptor[120] = {
        0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
        0x09, 0x02,        // USAGE (Mouse)
        0xa1, 0x01,        // COLLECTION (Application)
        0x09, 0x02,        //   USAGE (Mouse)
        0xa1, 0x02,        //   COLLECTION (Logical)
        0x09, 0x01,        //     USAGE (Pointer)
        0xa1, 0x00,        //     COLLECTION (Physical)
        // ------------------------------  Buttons
        0x05, 0x09,        //       USAGE_PAGE (Button)
        0x19, 0x01,        //       USAGE_MINIMUM (Button 1)
        0x29, 0x05,        //       USAGE_MAXIMUM (Button 5)
        0x15, 0x00,        //       LOGICAL_MINIMUM (0)
        0x25, 0x01,        //       LOGICAL_MAXIMUM (1)
        0x75, 0x01,        //       REPORT_SIZE (1)
        0x95, 0x03,        //       REPORT_COUNT (3 Buttons)
        0x81, 0x02,        //       INPUT (Data,Var,Abs)
        // ------------------------------  Padding
        0x75, 0x05,        //       REPORT_SIZE (8-3buttons 5)
        0x95, 0x01,        //       REPORT_COUNT (1)
        0x81, 0x03,        //       INPUT (Cnst,Var,Abs)
        // ------------------------------  X,Y position
        0x05, 0x01,        //       USAGE_PAGE (Generic Desktop)
        0x09, 0x30,        //       USAGE (X)
        0x09, 0x31,        //       USAGE (Y)
        0x15, 0x81,        //       LOGICAL_MINIMUM (-127)
        0x25, 0x7f,        //       LOGICAL_MAXIMUM (127)
        0x75, 0x08,        //       REPORT_SIZE (8)
        0x95, 0x02,        //       REPORT_COUNT (2)
        0x81, 0x06,        //       INPUT (Data,Var,Rel)
        0xa1, 0x02,        //       COLLECTION (Logical)
        // ------------------------------  Vertical wheel res multiplier
        0x09, 0x48,        //         USAGE (Resolution Multiplier)
        0x15, 0x00,        //         LOGICAL_MINIMUM (0)
        0x25, 0x01,        //         LOGICAL_MAXIMUM (1)
        0x35, 0x01,        //         PHYSICAL_MINIMUM (1)
        0x45, 0x08,        //         PHYSICAL_MAXIMUM (8)
        0x75, 0x02,        //         REPORT_SIZE (2)
        0x95, 0x01,        //         REPORT_COUNT (1)
        0xa4,              //         PUSH
        0xb1, 0x02,        //         FEATURE (Data,Var,Abs)
        // ------------------------------  Vertical wheel
        0x09, 0x38,        //         USAGE (Wheel)
        0x15, 0x81,        //         LOGICAL_MINIMUM (-127)
        0x25, 0x7f,        //         LOGICAL_MAXIMUM (127)
        0x35, 0x00,        //         PHYSICAL_MINIMUM (0)        - reset physical
        0x45, 0x00,        //         PHYSICAL_MAXIMUM (0)
        0x75, 0x08,        //         REPORT_SIZE (8)
        0x95, 0x01,        //         REPORT_COUNT (1)
        0x81, 0x06,        //         INPUT (Data,Var,Rel)
        0xc0,              //       END_COLLECTION
        0xa1, 0x02,        //       COLLECTION (Logical)
        // ------------------------------  Horizontal wheel res multiplier
        0x09, 0x48,        //         USAGE (Resolution Multiplier)
        0xb4,              //         POP
        0xb1, 0x02,        //         FEATURE (Data,Var,Abs)
        // ------------------------------  Padding for Feature report
        0x35, 0x00,        //         PHYSICAL_MINIMUM (0)        - reset physical
        0x45, 0x00,        //         PHYSICAL_MAXIMUM (0)
        0x75, 0x04,        //         REPORT_SIZE (4)
        0xb1, 0x03,        //         FEATURE (Cnst,Var,Abs)
        // ------------------------------  Horizontal wheel
        0x05, 0x0c,        //         USAGE_PAGE (Consumer Devices)
        0x0a, 0x38, 0x02,  //         USAGE (AC Pan)
        0x15, 0x81,        //         LOGICAL_MINIMUM (-127)
        0x25, 0x7f,        //         LOGICAL_MAXIMUM (127)
        0x75, 0x08,        //         REPORT_SIZE (8)
        0x81, 0x06,        //         INPUT (Data,Var,Rel)
        0xc0,              //       END_COLLECTION
        0xc0,              //     END_COLLECTION
        0xc0,              //   END_COLLECTION
        0xc0               // END_COLLECTION
};*/

const uint8_t REPORT_ID_MOUSE = 1;
const uint8_t REPORT_ID_MULTIPLIER = 2;

const uint8_t _hidReportDescriptor[] = {
    0x05, 0x01,                 // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,                 // Usage (Mouse)
    0xA1, 0x01,                 // Collection (Application)
    0x05, 0x01,                 //   Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,                 //   Usage (Mouse)
    0xA1, 0x02,                 //   Collection (Logical)
    0x85, REPORT_ID_MOUSE,      //     Report ID (REPORT_ID_MOUSE)
    0x09, 0x01,                 //     Usage (Pointer)
    0xA1, 0x00,                 //     Collection (Physical)
    0x05, 0x09,                 //       Usage Page (Button)
    0x19, 0x01,                 //       Usage Minimum (0x01)
    0x29, 0x08,                 //       Usage Maximum (0x08)
    0x95, 0x08,                 //       Report Count (8)
    0x75, 0x01,                 //       Report Size (1)
    0x25, 0x01,                 //       Logical Maximum (1)
    0x81, 0x02,                 //       Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,                 //       Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,                 //       Usage (X)
    0x09, 0x31,                 //       Usage (Y)
    0x95, 0x02,                 //       Report Count (2)
    0x75, 0x08,                 //       Report Size (8)
    0x15, 0x81,                 //         LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                 //         LOGICAL_MAXIMUM (127)
    0x81, 0x06,                 //       Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xA1, 0x02,                 //       Collection (Logical)
    0x85, REPORT_ID_MULTIPLIER, //         Report ID (REPORT_ID_MULTIPLIER)
    0x09, 0x48,                 //         Usage (Resolution Multiplier)
    0x95, 0x01,                 //         Report Count (1)
    0x75, 0x04,                 //         Report Size (4)
    0x15, 0x00,                 //         Logical Minimum (0)
    0x25, 0x01,                 //         Logical Maximum (1)
    0x35, 0x01,                 //         Physical Minimum (1)
    0x45, 0x0f,                 //         Physical Maximum (RESOLUTION_MULTIPLIER)
    0xB1, 0x02,                 //         Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, REPORT_ID_MOUSE,      //         Report ID (REPORT_ID_MOUSE)
    0x09, 0x38,                 //         Usage (Wheel)
    0x35, 0x00,                 //         Physical Minimum (0)
    0x45, 0x00,                 //         Physical Maximum (0)
    0x15, 0x81,                 //         LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                 //         LOGICAL_MAXIMUM (127)
    0x75, 0x08,                 //         Report Size (8)
    0x81, 0x06,                 //         Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                       //       End Collection
    0xA1, 0x02,                 //       Collection (Logical)
    0x85, REPORT_ID_MULTIPLIER, //         Report ID (REPORT_ID_MULTIPLIER)
    0x09, 0x48,                 //         Usage (Resolution Multiplier)
    0x75, 0x04,                 //         Report Size (4)
    0x15, 0x00,                 //         Logical Minimum (0)
    0x25, 0x01,                 //         Logical Maximum (1)
    0x35, 0x01,                 //         Physical Minimum (1)
    0x45, 0x0F,                 //         Physical Maximum (RESOLUTION_MULTIPLIER)
    0xB1, 0x02,                 //         Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x35, 0x00,                 //         Physical Minimum (0)
    0x45, 0x00,                 //         Physical Maximum (0)
    // 0x75, 0x04,                 //         Report Size (4)
    // 0xB1, 0x03,                 //         Feature (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, REPORT_ID_MOUSE, //         Report ID (REPORT_ID_MOUSE)
    0x05, 0x0C,            //         Usage Page (Consumer)
    0x15, 0x81,            //         LOGICAL_MINIMUM (-127)
    0x25, 0x7f,            //         LOGICAL_MAXIMUM (127)
    0x75, 0x08,            //         Report Size (8)
    0x0A, 0x38, 0x02,      //         Usage (AC Pan)
    0x81, 0x06,            //         Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                  //       End Collection
    0xC0,                  //     End Collection
    0xC0,                  //   End Collection
    0xC0                   // End Collection
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
    // BleMouse *bleMouseInstance = (BleMouse *)pvParameter;
    BLEDevice::init(this->deviceName);
    // BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
    //  BLEDevice::setMTU(BLE_MTU_SIZE);
    //  setDefaultPhy(ESP_BLE_GAP_PHY_2M_PREF_MASK, ESP_BLE_GAP_PHY_2M_PREF_MASK);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(this->connectionStatus);

    this->hid = new BLEHIDDevice(pServer);
    this->inputMouse = this->hid->inputReport(REPORT_ID_MOUSE); // <-- input REPORTID from report map
    this->featWheelMul = this->hid->featureReport(REPORT_ID_MULTIPLIER);
    this->featWheelMul->setCallbacks(new wheelFeatCallback());

    this->connectionStatus->inputMouse = this->inputMouse;

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    ESP_LOGI("####", "report len: %d", sizeof(_hidReportDescriptor));
    this->hid->reportMap((uint8_t *)_hidReportDescriptor, sizeof(_hidReportDescriptor));

    this->hid->manufacturer()->setValue(this->deviceManufacturer);
    // this->hid->pnp(0x02, 0x1915, 0xEEF1, 0x0100);
    // this->hid->pnp(0x01,0x02e5,0xabcc,0x0110);
    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    this->hid->hidInfo(0x00, 0x01);

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->setScanResponse(false);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());

    this->onStarted(pServer);

    this->hid->startServices();
    this->hid->setBatteryLevel(this->batteryLevel);
    pAdvertising->start();
}

void BleMouse::end(void)
{
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
        this->inputMouse->setValue(m, 5);
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

wheelFeatCallback::wheelFeatCallback()
{
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
        m[0] = 0xFF;
        // m[1] = 0xFF;
        pCharacteristic->setValue(m, 0x1);
        pCharacteristic->notify();
    }
}

void wheelFeatCallback::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string buff = pCharacteristic->getValue();
    ESP_LOGI("####", "onWrite Feature report data: %d", buff.length());
}

