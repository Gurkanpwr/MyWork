/*
 * Keyboard_HID.h
 *
 *  Created on: Apr 27, 2023
 *      Author: gurka
 */
#include "usb_device.h"
#include "usbd_hid.h"

#ifndef INC_KEYBOARD_HID_H_
#define INC_KEYBOARD_HID_H_
void USB_Keyboard_SendChar(char ch);


void USB_Keyboard_SendString(char * s);
#endif /* INC_KEYBOARD_HID_H_ */
