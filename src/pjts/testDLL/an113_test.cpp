#include <iostream>

#include "ftd2xx.h"


//////////////////////////////////////////////////////////////////
// Define the global variables and const variables
//////////////////////////////////////////////////////////////////
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN = '\x24';
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT = '\x11';
const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN = '\x22';
FT_STATUS ftStatus; //Status defined in D2XX to indicate operation result
FT_HANDLE ftHandle; //Handle of FT2232H device port
BYTE OutputBuffer[1024]; //Buffer to hold MPSSE commands and data to be sent to FT2232H
BYTE InputBuffer[1024]; //Buffer to hold Data bytes to be read from FT2232H
DWORD dwClockDivisor = 0x0095; //Value of clock divisor, SCL Frequency = 60/((1+0x0095)*2) (MHz) = 200khz
DWORD dwNumBytesToSend = 0; //Index of output buffer
DWORD dwNumBytesSent = 0, dwNumBytesRead = 0, dwNumInputBuffer = 0;

//////////////////////////////////////////////////////////////////////////////////////
// Below function will setup the START condition for I2C bus communication. First, set SDA, SCL high and ensure hold time
// requirement by device is met. Second, set SDA low, SCL high and ensure setup time requirement met. Finally, set SDA, SCL low
////////////////////////////////////////////////////////////////////////////////////////
void HighSpeedSetI2CStart(void)
{
	DWORD dwCount;
	for (dwCount = 0; dwCount < 4; dwCount++) // Repeat commands to ensure the minimum period of the start hold time ie 600ns is achieved
	{
		OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		OutputBuffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit ‘1’, GPIOL0 at bit ‘0’
		OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ‘1’, other pins as input with bit ‘0’
	}
	for (dwCount = 0; dwCount < 4; dwCount++) // Repeat commands to ensure the minimum period of the start setup time ie 600ns is achieved
	{
		OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		OutputBuffer[dwNumBytesToSend++] = '\x01'; //Set SDA low, SCL high, WP disabled by SK at bit ‘1’, DO, GPIOL0 at bit ‘0’
		OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ‘1’, other pins as input with bit ‘0’
	}
	OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	OutputBuffer[dwNumBytesToSend++] = '\x00'; //Set SDA, SCL low, WP disabled by SK, DO, GPIOL0 at bit ‘0’
	OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ‘1’, other pins as input with bit ‘0’
}


//////////////////////////////////////////////////////////////////////////////////////
// Below function will setup the STOP condition for I2C bus communication. First, set SDA low, SCL high and ensure setup time
// requirement by device is met. Second, set SDA, SCL high and ensure hold time requirement met. Finally, set SDA, SCL as input
// to tristate the I2C bus.
////////////////////////////////////////////////////////////////////////////////////////
void HighSpeedSetI2CStop(void)
{
	DWORD dwCount;
	for (dwCount = 0; dwCount < 4; dwCount++) // Repeat commands to ensure the minimum period of the stop setup time ie 600ns is achieved
	{
		OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		OutputBuffer[dwNumBytesToSend++] = '\x01'; //Set SDA low, SCL high, WP disabled by SK at bit ‘1’, DO, GPIOL0 at bit ‘0’
		OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ‘1’, other pins as input with bit ‘0’
	}
	for (dwCount = 0; dwCount < 4; dwCount++) // Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
	{
		OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		OutputBuffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit ‘1’, GPIOL0 at bit ‘0’
		OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ‘1’, other pins as input with bit ‘0’
	}
	//Tristate the SCL, SDA pins
	OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	OutputBuffer[dwNumBytesToSend++] = '\x00'; //Set WP disabled by GPIOL0 at bit ‘0’
	OutputBuffer[dwNumBytesToSend++] = '\x10'; //Set GPIOL0 pins as output with bit ‘1’, SK, DO and other pins as input with bit ‘0’
}

//////////////////////////////////////////////////////////////////////////////////////
// Below function will send a data byte to I2C-bus EEPROM 24LC256, then check if the ACK bit sent from 24LC256 device can be received.
// Return true if data is successfully sent and ACK bit is received. Return false if error during sending data or ACK bit can’t be received
//////////////////////////////////////////////////////////////////////////////////////
BOOL SendByteAndCheckACK(BYTE dwDataSend)
{
	FT_STATUS ftStatus = FT_OK;
	OutputBuffer[dwNumBytesToSend++] = MSB_FALLING_EDGE_CLOCK_BYTE_OUT; //Clock data byte out on –ve Clock Edge MSB first
	OutputBuffer[dwNumBytesToSend++] = '\x00';
	OutputBuffer[dwNumBytesToSend++] = '\x00'; //Data length of 0x0000 means 1 byte data to clock out
	OutputBuffer[dwNumBytesToSend++] = dwDataSend; //Add data to be send
	//Get Acknowledge bit from EEPROM
	OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	OutputBuffer[dwNumBytesToSend++] = '\x00'; //Set SCL low, WP disabled by SK, GPIOL0 at bit ‘0’
	OutputBuffer[dwNumBytesToSend++] = '\x11'; //Set SK, GPIOL0 pins as output with bit ‘1’, DO and other pins as input with bit ‘0’
	OutputBuffer[dwNumBytesToSend++] = MSB_RISING_EDGE_CLOCK_BIT_IN; //Command to scan in ACK bit , -ve clock Edge MSB first
	OutputBuffer[dwNumBytesToSend++] = '\x0'; //Length of 0x0 means to scan in 1 bit
	OutputBuffer[dwNumBytesToSend++] = '\x87'; //Send answer back immediate command
	ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
	dwNumBytesToSend = 0; //Clear output buffer
	//Check if ACK bit received, may need to read more times to get ACK bit or fail if timeout
	ftStatus = FT_Read(ftHandle, InputBuffer, 1, &dwNumBytesRead); //Read one byte from device receive buffer
	if ((ftStatus != FT_OK) || (dwNumBytesRead == 0))
	{
		return FALSE; /*Error, can't get the ACK bit from EEPROM */
	}
	else
		if (((InputBuffer[0] & BYTE('\x1')) != BYTE('\x0'))) //Check ACK bit 0 on data byte read out
		{
			return FALSE; /*Error, can't get the ACK bit from EEPROM */
		}
	OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
	OutputBuffer[dwNumBytesToSend++] = '\x02'; //Set SDA high, SCL low, WP disabled by SK at bit '0', DO, GPIOL0 at bit '1'
	OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ‘1’, other pins as input with bit ‘0’
	return TRUE;
}

int main()
{
	DWORD dwCount;
	//Try to open the FT2232H device port and get the valid handle for subsequent access
	char SerialNumBuf[64];
	ftStatus = FT_ListDevices((PVOID)0, &SerialNumBuf, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
	ftStatus = FT_OpenEx((PVOID)SerialNumBuf, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
	if (ftStatus == FT_OK)
	{ // Port opened successfully
		ftStatus |= FT_ResetDevice(ftHandle); //Reset USB device
		//Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
		ftStatus |= FT_GetQueueStatus(ftHandle, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer
		if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
			FT_Read(ftHandle, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
		ftStatus |= FT_SetUSBParameters(ftHandle, 65536, 65535); //Set USB request transfer size
		ftStatus |= FT_SetChars(ftHandle, false, 0, false, 0); //Disable event and error characters
		ftStatus |= FT_SetTimeouts(ftHandle, 0, 5000); //Sets the read and write timeouts in milliseconds for the FT2232H
		ftStatus |= FT_SetLatencyTimer(ftHandle, 16); //Set the latency timer
		ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00); //Reset controller
		ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x02); //Enable MPSSE mode
		if (ftStatus != FT_OK)

		{ /*Error on initialize MPSEE of FT2232H*/
		}
		Sleep(50); // Wait for all the USB stuff to complete and work
		//////////////////////////////////////////////////////////////////
		// Below codes will synchronize the MPSSE interface by sending bad command ‘xAA’ and checking if the echo command followed by
		// bad command ‘AA’ can be received, this will make sure the MPSSE interface enabled and synchronized successfully
		//////////////////////////////////////////////////////////////////
		OutputBuffer[dwNumBytesToSend++] = '\xAA'; //Add BAD command ‘xAA’
		ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the BAD commands
		dwNumBytesToSend = 0; //Clear output buffer
		do {
			ftStatus = FT_GetQueueStatus(ftHandle, &dwNumInputBuffer); // Get the number of bytes in the device input buffer
		} while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK)); //or Timeout
		bool bCommandEchod = false;
		ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from input buffer
		for (dwCount = 0; dwCount < dwNumBytesRead - 1; dwCount++) //Check if Bad command and echo command received
		{
			if ((InputBuffer[dwCount] == BYTE('\xFA')) && (InputBuffer[dwCount + 1] == BYTE('\xAA')))
			{
				bCommandEchod = true;
				break;
			}
		}
		if (bCommandEchod == false)
		{ /*Error, can’t receive echo command , fail to synchronize MPSSE interface;*/
		}
		////////////////////////////////////////////////////////////////////
		//Configure the MPSSE settings for I2C communication with 24LC256
		//////////////////////////////////////////////////////////////////
		OutputBuffer[dwNumBytesToSend++] = '\x8A'; //Ensure disable clock divide by 5 for 60Mhz master clock
		OutputBuffer[dwNumBytesToSend++] = '\x97'; //Ensure turn off adaptive clocking
		OutputBuffer[dwNumBytesToSend++] = '\x8C'; //Enable 3 phase data clock, used by I2C to allow data on both clock edges
		ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
		dwNumBytesToSend = 0; //Clear output buffer
		OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output
		OutputBuffer[dwNumBytesToSend++] = '\x03'; //Set SDA, SCL high, WP disabled by SK, DO at bit ‘1’, GPIOL0 at bit ‘0’
		OutputBuffer[dwNumBytesToSend++] = '\x13'; //Set SK,DO,GPIOL0 pins as output with bit ’, other pins as input with bit ‘’
		// The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
		// SK frequency = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)
		OutputBuffer[dwNumBytesToSend++] = '\x86'; //Command to set clock divisor
		OutputBuffer[dwNumBytesToSend++] = dwClockDivisor & '\xFF'; //Set 0xValueL of clock divisor
		OutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & '\xFF'; //Set 0xValueH of clock divisor
		ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
		dwNumBytesToSend = 0; //Clear output buffer
		Sleep(20); //Delay for a while
		//Turn off loop back in case
		OutputBuffer[dwNumBytesToSend++] = '\x85'; //Command to turn off loop back of TDI/TDO connection
		ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
		dwNumBytesToSend = 0; //Clear output buffer
		Sleep(30); //Delay for a while
	}
	return 0;
}
















