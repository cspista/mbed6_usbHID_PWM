# USB generic HID PWM demo
## project name: mbed6_usbHID_PWM
Tested by: **Keil Studio** 1.50 and **Mbed-OS** 6.16.0

This program implements a general USB HID device, from which the value of the voltage measured 
at the analog input A0 and the status of the user pushbutton can be queried from the PC. 
By sending a specific command, we can control the state of the built-in LED1, as well as
we can control the brightness of LED2 (connected to D3) by PWM.

We are sending and receiving packages of 8 bytes. 
In the case of the packages received from the PC only the first two bytes have meaning:
the firs byte controls the state of LED1 (0: off, 1: on)
the second byte controls the brightness of LED2 (0-100: duty cycle)
The package sent as answer are reports:

first byte: BUTTON1 state (0: released, 1: pressed) 
4th-5th bytes: ADC value in high endian (MSB first) order

                                 |

The **USBHID** class provides generic HID (Human Interface Device) functionality over the USB channel. 
In the case of generic HID devices, the HID report descriptor specifies 
only specifies the amount of data transferred. The interpretation of the data, however, is left to the firmware 
loaded into the microcontroller and the application running on the PC. In this case, only the method 
of transmission is standard, but the use of the data is unique and manufacturer-specific.
This means, that we need an application on our computer that can manage and understand the USBHID 
data traffic of the unique device we created. It can even be a script, for which we can use the **pywinusb ** 
extension of the Python interpreter. Another option is the [hidapi library](https://github.com/libusb/hidapi), 
which can be used from C/C++ console applications or graphical applications.




## Hardware requirements
* NUCLEO-F446RE development board
* One 10 k potentiometer which divides the 3,3 V power voltage. The slider is connected to A0 (PA_0) analog input.
* USB connector breakout for the STM32F446RE USBFS channel
* USB connection to a PC through the built-in ST-Link v2-1

## Wiring instructions for the NUCLEO-F446RE board

There is no USB connector for the USBFS channel of the target CPU therefore we have to prepare connection
by using an USB cable and an USB socket breakout card.

| USB breakout  | NUCLEO-F446RE pin   |
|:-------:|:-----: |
|  GND    |GND     | 
|  ID     | --     | 
|  D+     | PA_12  |
|  D-     | PA_11  | 
|  VBUS   | 5V     | 



![](./images/usbhid_pwm_wiring.png)


## Software requirements
* Start with an empty mbed 6 project
* Edit the main.cpp source file
* Configure the project by adding an **mbed_app.json** file in which
    * We have to configure **usb_speed** parameter
    * We have to add **USBDEVICE** to the device list of the target

Finally the **mbed_app.json** sholuld look like this: 

```
{
    "config": {
        "usb_speed": {
            "help": "USE_USB_OTG_FS or USE_USB_OTG_HS or USE_USB_HS_IN_FS",
            "value": "USE_USB_OTG_FS"
        }
    },
    "target_overrides": {
        "*": {
            "target.device_has_add": ["USBDEVICE"]
        }
    }
}
```
## mbed6_usbHID_PWM results

### Using with a graphics application 

This simple Generic HID demo is based on the 
example program shown in the Cypress AN82072 Application Note.

Due to the similarities, you can use the graphical PC application
from the published in the downloadable software package of the above 
mentioned Application Note, if you set the Vid/PId valuaes of our device 
(VID = 0x1234, PID = 0x0006). 

Note, that besides the Generic Hid Ui.exe executable, you also need 
the dynamic library cyUSB.dll as well (freom the same ZIP package)

![](./images/mbed6_usbhid_pwm.png)

### Using witk Python script

Another possibility is the usage of Python (we have to install the pywinusb helper module)
The attached Python script (usbhid_pwm.py) is a console application which provides 
a menu for controlling the USB communication with the mbed card.

![](./images/usbhid_pwm_py.png)