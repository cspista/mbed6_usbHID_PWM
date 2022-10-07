/* mbed6_usbHID_PWM
 *
 * This is a very simple Generic HID demo, which is based on the 
 * example program shown in the Cypress AN82072 Application Note.
 * Due to the similarities, you can use the graphical PC application
 * from the published in the downloadable software package of the above 
 * mentioned Application Note, if you set the Vid/PId valuaes of aur device 
 * (VID = 0x1234, PID = 0x0006). 
 * Note, that besides the Generic Hid Ui.exe executable, you also need 
 * the dynamic library cyUSB.dll as well (freom the same ZIP package)
 */

#include "mbed.h"
#include "USBHID.h"

//We declare a USBHID device. By default input and output reports are 8 bytes long.
USBHID hid(true,8, 8, 0x1234, 0x0006, 0x0001); // Vid/Pid: Mbed Custom Human Interface Device

HID_REPORT send_report = {
    .length = 8,
    .data = {0}
};
HID_REPORT recv_report = {
    .length = 8,
    .data = {0}
};

DigitalOut LED_1(LED1);        //Buitin LED at PA5
PwmOut LED_2(D3);              //External LED anode at D3 (PB3)
DigitalOut myGND(D4);          //Just for lazyness: LED cathode
DigitalIn SW1(BUTTON1,PullUp); //Builtin button at PC13
AnalogIn adc(A0);              //Analog input at A0 (PA0)

int main(void) {
    LED_1 = 0;
    LED_2.period_ms(20);
    myGND = 0;
    while (1) {
        uint16_t raw = adc.read_u16();                  //Read ADC (A0 chan)
        for (int i = 0; i < send_report.length; i++)    //Fill the report
            send_report.data[i] = 0x00;
        send_report.data[0] = !SW1.read();
        send_report.data[3] = (raw>>8);
        send_report.data[4] = (raw & 0xff);
        hid.send(&send_report);                         //Send the report

        if(hid.read_nb(&recv_report)) {                  //try to read a msg
            LED_1 = recv_report.data[0];
            LED_2.write(recv_report.data[1]*0.01f);
        }
    }
}