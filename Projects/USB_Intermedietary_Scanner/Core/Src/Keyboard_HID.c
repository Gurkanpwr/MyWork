/*
 * Include this code in your project.
 * You can also add this and the header file to your project.
 * Make sure to include header file in main.c
 * Some actions are based on English keyboard layout.
 * Only letters and symbols '.', "space", "enter" '!' and '?' are implemented.
 * Add cases in switch statement at line 38 for additional symbols and actions
*/

#include "Keyboard_HID.h"
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceHS;
static uint8_t HID_buffer[8] = { 0 };

// Send character as a single key press
void USB_Keyboard_SendChar(char ch)
{
	uint8_t ret;

	// Check if lower or upper case
	if(ch >= 'a' && ch <= 'z')
	{
		HID_buffer[0] = 0;
		// convert ch to HID letter, starting at a = 4
		HID_buffer[2] = (uint8_t)(4 + (ch - 'a'));
	}
	else if(ch >= 'A' && ch <= 'Z')
	{
		// Add left shift
		HID_buffer[0] = 2;
		// convert ch to lower case
		ch = ch - ('A'-'a');
		// convert ch to HID letter, starting at a = 4
		HID_buffer[2] = (uint8_t)(4 + (ch - 'a'));
	}
	else // not a letter
	{
		switch(ch)
		{
			case ' ':
				HID_buffer[2] = 44;
				break;
			case '.':
				HID_buffer[2] = 55;
				break;
			case '\n':
				HID_buffer[2] = 40;
				break;
			case '!':
				//combination of shift modifier and key
				HID_buffer[0] = 2;	// shift
				HID_buffer[2] = 30; // number 1
				break;
			case '?':
				//combination of shift modifier and key
				HID_buffer[0] = 2;	// shift
				HID_buffer[2] = 56; // key '/'
				break;
			case '=':
				HID_buffer[0]=0; //shift
				HID_buffer[2]= 46;
				break;
			case '1':
				HID_buffer[2]=30;
				break;
			case '2':
				HID_buffer[2]=31;
				break;
			case '3':
				HID_buffer[2]=32;
				break;
			case '4':
				HID_buffer[2]=33;
				break;
			case '5':
				HID_buffer[2]=34;
				break;
			case '6':
				HID_buffer[2]=35;
				break;
			case '7':
				HID_buffer[2]=36;
				break;
			case '8':
				HID_buffer[2]=37;
				break;
			case '9':
				HID_buffer[2]=38;
				break;
			case '0':
				HID_buffer[2]=39;
				break;
			default:
				HID_buffer[2] =	0;
		}
	}

	// press keys
	ret = USBD_HID_SendReport(&hUsbDeviceHS, HID_buffer, 8);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}
	HAL_Delay(15);

	// release keys
	HID_buffer[0] = 0;
	HID_buffer[2] = 0;
	ret = USBD_HID_SendReport(&hUsbDeviceHS, HID_buffer, 8);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}
	HAL_Delay(15);
}

// Send string as letters
void USB_Keyboard_SendString(char * s)
{
	uint8_t i = 0;

	while(*(s+i))
	{
		USB_Keyboard_SendChar(*(s+i));
		i++;
	}
}
