#define FT_VER_MAJOR  1
#define FT_VER_MINOR  0
#define FT_VER_BUILD  1
#define WIN32
#define _WINDOWS
#define _USRDLL
#define FTDIMPSSE_EXPORTS

#ifndef FT_VER_MAJOR
#define FT_VER_MAJOR		1
#endif // FT_VER_MAJOR
#ifndef FT_VER_MINOR
#define FT_VER_MINOR		0
#endif // FT_VER_MINOR
#ifndef FT_VER_BUILD
#define FT_VER_BUILD		5
#endif // FT_VER_BUILD

/******************************************************************************/
/*								Macro defines								  */
/******************************************************************************/
/* Macros to be called before starting and after ending communication over a MPSSE channel.
Implement the lock/unlock only if really required, otherwise keep as placeholders */
#define LOCK_CHANNEL(arg)	{;}
#define UNLOCK_CHANNEL(arg)	{;}

#define MIN_CLOCK_RATE 					0
#define MAX_CLOCK_RATE 					30000000

#define MIN_LATENCY_TIMER       		0
#define MAX_LATENCY_TIMER       		255
#define USB_INPUT_BUFFER_SIZE			65536
#define USB_OUTPUT_BUFFER_SIZE			65536
#define DISABLE_EVENT					0
#define DISABLE_CHAR					0
#define DEVICE_READ_TIMEOUT_INFINITE    0
#define DEVICE_READ_TIMEOUT 			5000
#define DEVICE_WRITE_TIMEOUT 			5000
#define INTERFACE_MASK_IN				0x00
#define INTERFACE_MASK_OUT				0x01
#define RESET_INTERFACE					0
#define ENABLE_MPSSE					0X02

/*MPSSE Control Commands*/
#define MPSSE_CMD_SET_DATA_BITS_LOWBYTE		0x80
#define MPSSE_CMD_SET_DATA_BITS_HIGHBYTE	0x82
#define MPSSE_CMD_GET_DATA_BITS_LOWBYTE		0x81
#define MPSSE_CMD_GET_DATA_BITS_HIGHBYTE	0x83

#define MPSSE_CMD_SEND_IMMEDIATE			0x87
#define MPSSE_CMD_ENABLE_3PHASE_CLOCKING	0x8C
#define MPSSE_CMD_DISABLE_3PHASE_CLOCKING	0x8D
#define MPSSE_CMD_ENABLE_DRIVE_ONLY_ZERO	0x9E

/*MPSSE Data Command - LSB First */
#define MPSSE_CMD_DATA_LSB_FIRST			0x08

/*MPSSE Data Commands - bit mode - MSB first */
#define MPSSE_CMD_DATA_OUT_BITS_POS_EDGE	0x12
#define MPSSE_CMD_DATA_OUT_BITS_NEG_EDGE	0x13
#define MPSSE_CMD_DATA_IN_BITS_POS_EDGE		0x22
#define MPSSE_CMD_DATA_IN_BITS_NEG_EDGE		0x26
#define MPSSE_CMD_DATA_BITS_IN_POS_OUT_NEG_EDGE	0x33
#define MPSSE_CMD_DATA_BITS_IN_NEG_OUT_POS_EDGE	0x36


/*MPSSE Data Commands - byte mode - MSB first */
#define MPSSE_CMD_DATA_OUT_BYTES_POS_EDGE	0x10
#define MPSSE_CMD_DATA_OUT_BYTES_NEG_EDGE	0x11
#define MPSSE_CMD_DATA_IN_BYTES_POS_EDGE	0x20
#define MPSSE_CMD_DATA_IN_BYTES_NEG_EDGE	0x24
#define MPSSE_CMD_DATA_BYTES_IN_POS_OUT_NEG_EDGE	0x31
#define MPSSE_CMD_DATA_BYTES_IN_NEG_OUT_POS_EDGE	0x34


/*SCL & SDA directions*/
#define DIRECTION_SCLIN_SDAIN				0x10
#define DIRECTION_SCLOUT_SDAIN				0x11
#define DIRECTION_SCLIN_SDAOUT				0x12
#define DIRECTION_SCLOUT_SDAOUT				0x13

/*SCL & SDA values*/
#define VALUE_SCLLOW_SDALOW					0x00
#define VALUE_SCLHIGH_SDALOW				0x01
#define VALUE_SCLLOW_SDAHIGH				0x02
#define VALUE_SCLHIGH_SDAHIGH				0x03

/*Data size in bits*/
#define DATA_SIZE_8BITS						0x07
#define DATA_SIZE_1BIT						0x00

/* The I2C master should actually drive the SDA line only when the output is LOW. It should
tristate the SDA line when the output should be high. This tristating the SDA line during high
output is supported only in FT232H chip. This feature is called DriveOnlyZero feature and is
enabled when the following bit is set in the options parameter in function I2C_Init */
#define I2C_ENABLE_DRIVE_ONLY_ZERO	0x0002

#ifndef I2C_ENABLE_PIN_STATE_CONFIG
#define I2C_ENABLE_PIN_STATE_CONFIG 0x0010
#endif


/******************************************************************************/
/*								Type defines								  */
/******************************************************************************/

/*The middle layer is designed to have no knowledge of the legacy protocol used(I2C/JTAG/SPI)
but it will have to perform certain operations differently for supporting the different protocols. For
example, the sequence to initilize the hardware for the different protocols will be different. This is
why we need to have a parameter from the caller(top layer) to specify for which protocol is the
service(say, hardware initialization) required. The following enumeration is hence used uniformly
as the first parameter across all the middle layer APIs */
typedef enum FT_LegacyProtocol_t{SPI, I2C, JTAG}FT_LegacyProtocol;


#include <windows.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FTD2XX_API __declspec(dllimport)

/** @name FT_HANDLE 
 * An opaque value used as a handle to an opened FT device.
 */
typedef PVOID	FT_HANDLE;

/** @{
 * @name FT_STATUS 
 * @details Return status values for API calls. 
 */
typedef ULONG	FT_STATUS;


enum {
	FT_OK,
	FT_INVALID_HANDLE,
	FT_DEVICE_NOT_FOUND,
	FT_DEVICE_NOT_OPENED,
	FT_IO_ERROR,
	FT_INSUFFICIENT_RESOURCES,
	FT_INVALID_PARAMETER,
	FT_INVALID_BAUD_RATE,

	FT_DEVICE_NOT_OPENED_FOR_ERASE,
	FT_DEVICE_NOT_OPENED_FOR_WRITE,
	FT_FAILED_TO_WRITE_DEVICE,
	FT_EEPROM_READ_FAILED,
	FT_EEPROM_WRITE_FAILED,
	FT_EEPROM_ERASE_FAILED,
	FT_EEPROM_NOT_PRESENT,
	FT_EEPROM_NOT_PROGRAMMED,
	FT_INVALID_ARGS,
	FT_NOT_SUPPORTED,
	FT_OTHER_ERROR,
	FT_DEVICE_LIST_NOT_READY,
};
/** @} */

/** @name FT_SUCCESS Macro 
 * Macro to determine success of an API call.
 * @returns Non-zero for successful call.
 */
#define FT_SUCCESS(status) ((status) == FT_OK)

/** @{
 * @name FT_OpenEx Flags
 * @see FT_OpenEx
 */
#define FT_OPEN_BY_SERIAL_NUMBER	1
#define FT_OPEN_BY_DESCRIPTION		2
#define FT_OPEN_BY_LOCATION			4

#define FT_OPEN_MASK (FT_OPEN_BY_SERIAL_NUMBER | \
					  FT_OPEN_BY_DESCRIPTION | \
					  FT_OPEN_BY_LOCATION)
/** @} */

/** @{
 * @name FT_ListDevices Flags
 * @remarks Used in conjunction with FT_OpenEx Flags
 * @see FT_ListDevices
 * @see FT_OpenEx
 */
#define FT_LIST_NUMBER_ONLY			0x80000000
#define FT_LIST_BY_INDEX			0x40000000
#define FT_LIST_ALL					0x20000000

#define FT_LIST_MASK (FT_LIST_NUMBER_ONLY|FT_LIST_BY_INDEX|FT_LIST_ALL)
/** @} */

/** @{ 
 * @name Baud Rates
 * Standard baud rates supported by many implementations and applications.
 * @see FT_SetBaudRate
 */
#define FT_BAUD_300			300
#define FT_BAUD_600			600
#define FT_BAUD_1200		1200
#define FT_BAUD_2400		2400
#define FT_BAUD_4800		4800
#define FT_BAUD_9600		9600
#define FT_BAUD_14400		14400
#define FT_BAUD_19200		19200
#define FT_BAUD_38400		38400
#define FT_BAUD_57600		57600
#define FT_BAUD_115200		115200
#define FT_BAUD_230400		230400
#define FT_BAUD_460800		460800
#define FT_BAUD_921600		921600
/** @} */

/** @{ 
 * @name Word Lengths
 * @see FT_SetDataCharacteristics
 */
#define FT_BITS_8			(UCHAR) 8
#define FT_BITS_7			(UCHAR) 7
/** @} */

/** @{ 
 * @name Stop Bits
 * @see FT_SetDataCharacteristics
 */
#define FT_STOP_BITS_1		(UCHAR) 0
#define FT_STOP_BITS_2		(UCHAR) 2
/** @} */

/* * @name Parity
 * @see FT_SetDataCharacteristics
 * @{ 
 */
#define FT_PARITY_NONE		(UCHAR) 0
#define FT_PARITY_ODD		(UCHAR) 1
#define FT_PARITY_EVEN		(UCHAR) 2
#define FT_PARITY_MARK		(UCHAR) 3
#define FT_PARITY_SPACE		(UCHAR) 4
/** @} */

/** @{ 
 * @name Flow Control
 * @see FT_SetFlowControl
 */
#define FT_FLOW_NONE		0x0000
#define FT_FLOW_RTS_CTS		0x0100
#define FT_FLOW_DTR_DSR		0x0200
#define FT_FLOW_XON_XOFF	0x0400
/** @} */

/** @{ 
 * @name Purge rx and tx buffers
 * @see FT_Purge
 */
#define FT_PURGE_RX			1
#define FT_PURGE_TX			2
/** @} */

/** @{ 
 * @name Events
 * @see FT_SetEventNotification
 */
typedef void(*PFT_EVENT_HANDLER)(DWORD, DWORD);

#define FT_EVENT_RXCHAR			1
#define FT_EVENT_MODEM_STATUS	2
#define FT_EVENT_LINE_STATUS	4
/** @} */

/** @{ 
 * @name Timeouts
 * @see FT_SetTimeouts
 */
#define FT_DEFAULT_RX_TIMEOUT	300
#define FT_DEFAULT_TX_TIMEOUT	300
/** @} */

/** @{ 
 * @name Device Types
 * @details Known supported FTDI device types supported by this library.
 */
typedef ULONG	FT_DEVICE;

enum {
	FT_DEVICE_BM,
	FT_DEVICE_AM,
	FT_DEVICE_100AX,
	FT_DEVICE_UNKNOWN,
	FT_DEVICE_2232C,
	FT_DEVICE_232R,
	FT_DEVICE_2232H,
	FT_DEVICE_4232H,
	FT_DEVICE_232H,
	FT_DEVICE_X_SERIES,
	FT_DEVICE_4222H_0,
	FT_DEVICE_4222H_1_2,
	FT_DEVICE_4222H_3,
	FT_DEVICE_4222_PROG,
	FT_DEVICE_900,
	FT_DEVICE_930,
	FT_DEVICE_UMFTPD3A,
	FT_DEVICE_2233HP,
	FT_DEVICE_4233HP,
	FT_DEVICE_2232HP,
	FT_DEVICE_4232HP,
	FT_DEVICE_233HP,
	FT_DEVICE_232HP,
	FT_DEVICE_2232HA,
	FT_DEVICE_4232HA,
};
/** @} */

/** @{ 
 * @name Bit Modes
 * @see FT_SetBitMode FT_GetBitMode
 */
#define FT_BITMODE_RESET					0x00
#define FT_BITMODE_ASYNC_BITBANG			0x01
#define FT_BITMODE_MPSSE					0x02
#define FT_BITMODE_SYNC_BITBANG				0x04
#define FT_BITMODE_MCU_HOST					0x08
#define FT_BITMODE_FAST_SERIAL				0x10
#define FT_BITMODE_CBUS_BITBANG				0x20
#define FT_BITMODE_SYNC_FIFO				0x40
/** @} */

/** @{ 
 * @name FT232R CBUS Options EEPROM values
 */
#define FT_232R_CBUS_TXDEN					0x00	//	Tx Data Enable
#define FT_232R_CBUS_PWRON					0x01	//	Power On
#define FT_232R_CBUS_RXLED					0x02	//	Rx LED
#define FT_232R_CBUS_TXLED					0x03	//	Tx LED
#define FT_232R_CBUS_TXRXLED				0x04	//	Tx and Rx LED
#define FT_232R_CBUS_SLEEP					0x05	//	Sleep
#define FT_232R_CBUS_CLK48					0x06	//	48MHz clock
#define FT_232R_CBUS_CLK24					0x07	//	24MHz clock
#define FT_232R_CBUS_CLK12					0x08	//	12MHz clock
#define FT_232R_CBUS_CLK6					0x09	//	6MHz clock
#define FT_232R_CBUS_IOMODE					0x0A	//	IO Mode for CBUS bit-bang
#define FT_232R_CBUS_BITBANG_WR				0x0B	//	Bit-bang write strobe
#define FT_232R_CBUS_BITBANG_RD				0x0C	//	Bit-bang read strobe
#define FT_232R_CBUS0_RXF					0x0D	//	CBUS0 RXF#
#define FT_232R_CBUS1_TXE					0x0D	//	CBUS1 TXE#
#define FT_232R_CBUS2_RD					0x0D	//	CBUS2 RD#
#define FT_232R_CBUS3_WR					0x0D	//	CBUS3 WR#
/** @} */

/** @{ 
 * @name FT232H CBUS Options EEPROM values
 */
#define FT_232H_CBUS_TRISTATE				0x00	//	Tristate
#define FT_232H_CBUS_TXLED					0x01	//	Tx LED
#define FT_232H_CBUS_RXLED					0x02	//	Rx LED
#define FT_232H_CBUS_TXRXLED				0x03	//	Tx and Rx LED
#define FT_232H_CBUS_PWREN					0x04	//	Power Enable
#define FT_232H_CBUS_SLEEP					0x05	//	Sleep
#define FT_232H_CBUS_DRIVE_0				0x06	//	Drive pin to logic 0
#define FT_232H_CBUS_DRIVE_1				0x07	//	Drive pin to logic 1
#define FT_232H_CBUS_IOMODE					0x08	//	IO Mode for CBUS bit-bang
#define FT_232H_CBUS_TXDEN					0x09	//	Tx Data Enable
#define FT_232H_CBUS_CLK30					0x0A	//	30MHz clock
#define FT_232H_CBUS_CLK15					0x0B	//	15MHz clock
#define FT_232H_CBUS_CLK7_5					0x0C	//	7.5MHz clock
/** @} */

/** @{ 
 * @name FT X Series CBUS Options EEPROM values
 */
#define FT_X_SERIES_CBUS_TRISTATE			0x00	//	Tristate
#define FT_X_SERIES_CBUS_TXLED				0x01	//	Tx LED
#define FT_X_SERIES_CBUS_RXLED				0x02	//	Rx LED
#define FT_X_SERIES_CBUS_TXRXLED			0x03	//	Tx and Rx LED
#define FT_X_SERIES_CBUS_PWREN				0x04	//	Power Enable
#define FT_X_SERIES_CBUS_SLEEP				0x05	//	Sleep
#define FT_X_SERIES_CBUS_DRIVE_0			0x06	//	Drive pin to logic 0
#define FT_X_SERIES_CBUS_DRIVE_1			0x07	//	Drive pin to logic 1
#define FT_X_SERIES_CBUS_IOMODE				0x08	//	IO Mode for CBUS bit-bang
#define FT_X_SERIES_CBUS_TXDEN				0x09	//	Tx Data Enable
#define FT_X_SERIES_CBUS_CLK24				0x0A	//	24MHz clock
#define FT_X_SERIES_CBUS_CLK12				0x0B	//	12MHz clock
#define FT_X_SERIES_CBUS_CLK6				0x0C	//	6MHz clock
#define FT_X_SERIES_CBUS_BCD_CHARGER		0x0D	//	Battery charger detected
#define FT_X_SERIES_CBUS_BCD_CHARGER_N		0x0E	//	Battery charger detected inverted
#define FT_X_SERIES_CBUS_I2C_TXE			0x0F	//	I2C Tx empty
#define FT_X_SERIES_CBUS_I2C_RXF			0x10	//	I2C Rx full
#define FT_X_SERIES_CBUS_VBUS_SENSE			0x11	//	Detect VBUS
#define FT_X_SERIES_CBUS_BITBANG_WR			0x12	//	Bit-bang write strobe
#define FT_X_SERIES_CBUS_BITBANG_RD			0x13	//	Bit-bang read strobe
#define FT_X_SERIES_CBUS_TIMESTAMP			0x14	//	Toggle output when a USB SOF token is received
#define FT_X_SERIES_CBUS_KEEP_AWAKE			0x15	//	
/** @} */

/** @{ 
 * @name Driver Types
 */
#define FT_DRIVER_TYPE_D2XX		0
#define FT_DRIVER_TYPE_VCP		1
/** @} */

/** @{
 * @name FT_DEVICE_LIST_INFO_NODE Device Information Flags
 * @par Summary
 * These flags are used in the Flags member of FT_DEVICE_LIST_INFO_NODE to indicated the state of 
 * the device and speed of the device.
*/
enum {
	FT_FLAGS_OPENED = 1,
	FT_FLAGS_HISPEED = 2
};
/** @} */

extern "C" {
	typedef struct _ft_device_list_info_node {
		ULONG Flags;
		ULONG Type;
		ULONG ID;
		DWORD LocId;
		char SerialNumber[16];
		char Description[64];
		FT_HANDLE ftHandle;
	} FT_DEVICE_LIST_INFO_NODE;


}

extern "C" {

#define FTDIMPSSE_API __declspec(dllexport)
#define CAL_CONV	__stdcall

/* Uncomment the #define INFRA_DEBUG_ENABLE in makefile to enable debug messages */
#define MSG_EMERG	0 /*Used for emergency messages, usually those that precede a crash*/
#define MSG_ALERT	1 /*A situation requiring immediate action */
#define MSG_CRIT	2 /*Critical conditions, often related to serious hw/sw failures */
#define MSG_ERR		3 /*Used to report error conditions */
#define MSG_WARN	4 /*Warnings about problematic situations that do not, in themselves,
						create serious problems with the system */
#define MSG_NOTICE	5 /*Situations that are normal, but still worthy of note. */
#define MSG_INFO	6 /*Informational messages */
#define MSG_DEBUG	7 /*Used for debugging messages */  

#define likely(x)       (x)
#define unlikely(x)     (x)

    /* Macro to check null expression and exit if true */
#define CHECK_NULL(exp) {if (unlikely((exp) == NULL)){ printf("%s:%d:%s(): NULL" \
	" expression encountered\n", __FILE__, __LINE__, __FUNCTION__);exit(1);};};

/* Macro to check null expression and return if true */
#define CHECK_NULL_RET(exp){if (unlikely((exp) == NULL)){ printf("%s:%d:%s(): NULL" \
	" expression encountered\n", __FILE__, __LINE__, __FUNCTION__);\
	status=FT_INVALID_PARAMETER ; return(status);};};

/* Macro to check status  code and only print debug message */
#define CHECK_STATUS_NORET(exp) {if (unlikely((exp) != FT_OK)){DBG(MSG_ERR," status" \
	" != FT_OK\n"); Infra_DbgPrintStatus(exp);};};

/* Macro to check status  code and return if not FT_OK */
#define CHECK_STATUS(exp) {if (unlikely(exp!=FT_OK)){DBG(MSG_ERR," status \
	!= FT_OK\n");Infra_DbgPrintStatus(exp);return(exp);}else{;}};

	#define FN_ENTER	DBG(MSG_DEBUG,"Entering function\n");
	#define FN_EXIT		DBG(MSG_DEBUG,"Exiting function status=%u\n",(unsigned)status);
	#define FN_PT		DBG(MSG_DEBUG,"Path taken\n");
#define INFRA_SLEEP(exp)			Sleep(exp);

/* Memory allocating, freeing & copying macros -  */
#define INFRA_MALLOC(exp)			malloc(exp); \
	DBG(MSG_DEBUG,"INFRA_MALLOC %ubytes\n", exp);
#define INFRA_FREE(exp)				free(exp); \
	DBG(MSG_DEBUG,"INFRA_FREE 0x%x\n", exp);
#define INFRA_MEMCPY(dest, src, siz)	memcpy(dest, src, siz);\
	DBG(MSG_DEBUG,"INFRA_MEMCPY dest:0x%x src:0x%x size:0x%x\n", dest, src, siz);


/* Match types with WinTypes.h for non-windows platforms. */
typedef UCHAR   uint8;
typedef USHORT  uint16;
typedef ULONGLONG uint64;

typedef signed char   int8;
typedef signed short  int16;
typedef signed long long int64;


 // typedef BOOL	bool; // bool is defined already in C++

typedef unsigned int   uint32;
typedef signed int   int32;

typedef FT_STATUS (CAL_CONV *pfunc_FT_GetLibraryVersion)(LPDWORD lpdwVersion);
typedef FT_STATUS (CAL_CONV *pfunc_FT_GetNumChannel)(LPDWORD lpdwNumDevs);
typedef FT_STATUS (CAL_CONV *pfunc_FT_GetDeviceInfoList)(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs);
typedef FT_STATUS (CAL_CONV *pfunc_FT_Open) (int iDevice, FT_HANDLE *ftHandle);
typedef FT_STATUS (CAL_CONV *pfunc_FT_Close) (FT_HANDLE ftHandle);
typedef FT_STATUS (CAL_CONV *pfunc_FT_ResetDevice) (FT_HANDLE ftHandle);
typedef FT_STATUS (CAL_CONV *pfunc_FT_Purge) (FT_HANDLE ftHandle, DWORD dwMask);
typedef FT_STATUS (CAL_CONV *pfunc_FT_SetUSBParameters) (FT_HANDLE ftHandle, DWORD dwInTransferSize, DWORD dwOutTransferSize);
typedef FT_STATUS (CAL_CONV *pfunc_FT_SetChars) (FT_HANDLE ftHandle, UCHAR uEventCh, UCHAR uEventChEn, UCHAR uErrorCh, UCHAR uErrorChEn);
typedef FT_STATUS (CAL_CONV *pfunc_FT_SetTimeouts) (FT_HANDLE ftHandle, DWORD dwReadTimeout, DWORD dwWriteTimeout);
typedef FT_STATUS (CAL_CONV *pfunc_FT_SetLatencyTimer) (FT_HANDLE ftHandle, UCHAR ucTimer);
typedef FT_STATUS (CAL_CONV *pfunc_FT_GetLatencyTimer) (FT_HANDLE ftHandle, UCHAR *ucTimer);
typedef FT_STATUS (CAL_CONV *pfunc_FT_SetBitmode) (FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucMode);
typedef FT_STATUS (CAL_CONV *pfunc_FT_GetQueueStatus) (FT_HANDLE ftHandle, LPDWORD lpdwAmountInRxQueue);
typedef FT_STATUS (CAL_CONV *pfunc_FT_Read) (FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpdwBytesReturned);
typedef FT_STATUS (CAL_CONV *pfunc_FT_Write) (FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToWrite, LPDWORD lpdwBytesWritten);
typedef FT_STATUS (CAL_CONV *pfunc_FT_GetDeviceInfo)(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description, LPVOID Dummy);

typedef struct InfraFunctionPtrLst_t
{
	pfunc_FT_GetLibraryVersion p_FT_GetLibraryVersion;
	pfunc_FT_GetNumChannel p_FT_GetNumChannel;
	pfunc_FT_GetDeviceInfoList p_FT_GetDeviceInfoList;
	pfunc_FT_Open p_FT_Open;
	pfunc_FT_Close p_FT_Close;
	pfunc_FT_ResetDevice p_FT_ResetDevice;
	pfunc_FT_Purge p_FT_Purge;
	pfunc_FT_SetUSBParameters p_FT_SetUSBParameters;
	pfunc_FT_SetChars p_FT_SetChars;
	pfunc_FT_SetTimeouts p_FT_SetTimeouts;
	pfunc_FT_SetLatencyTimer p_FT_SetLatencyTimer;
	pfunc_FT_GetLatencyTimer p_FT_GetLatencyTimer;
	pfunc_FT_SetBitmode p_FT_SetBitmode;
	pfunc_FT_GetQueueStatus p_FT_GetQueueStatus;
	pfunc_FT_Read p_FT_Read;
	pfunc_FT_Write p_FT_Write;
	pfunc_FT_GetDeviceInfo p_FT_GetDeviceInfo;
} InfraFunctionPtrLst;

	extern HANDLE hdll_d2xx;


extern InfraFunctionPtrLst varFunctionPtrLst;
    
/******************************************************************************/
/*								Function declarations						  */
/******************************************************************************/
// FT_STATUS Infra_DbgPrintStatus(FT_STATUS status);
// FT_STATUS Infra_Delay(uint64 delay);

}


extern "C" {

#define MID_COMMENTED_AFTER_REVIEW_COMMENT  			0
#define MID_COMMENTED_AFTER_REVIEW_COMMENT_ENTRY_EXIT 	0

#define MID_NO_CHANNEL_FOUND	0

#define MID_NO_MPSSE					0
#define MID_MPSSE_AVAILABLE				1

#define MID_MAX_IN_BUF_SIZE         	4096

#define MID_ECHO_COMMAND_ONCE			0
#define MID_ECHO_COMMAND_CONTINUOUSLY   1
#define MID_ECHO_CMD_1					0xAA
#define MID_ECHO_CMD_2					0xAB
#define MID_BAD_COMMAND_RESPONSE        0xFA
#define MID_CMD_NOT_ECHOED				0
#define MID_CMD_ECHOED					1

/*clock*/
#define MID_SET_LOW_BYTE_DATA_BITS_CMD	0x80
#define MID_GET_LOW_BYTE_DATA_BITS_CMD	0x81
#define MID_SET_HIGH_BYTE_DATA_BITS_CMD	0x82
#define MID_GET_HIGH_BYTE_DATA_BITS_CMD	0x83
#define MID_SET_CLOCK_FREQUENCY_CMD		0x86
#define MID_SET_LOW_BYTE_DATA_BITS_DATA 0x13
#define MID_SET_HIGH_BYTE_DATA_BITS_DATA 0x0F

#define MID_6MHZ						6000000
#define MID_30MHZ						30000000

#define DISABLE_CLOCK_DIVIDE			0x8A
#define ENABLE_CLOCK_DIVIDE				0x8B

#define MID_LOOPBACK_FALSE				0
#define MID_LOOPBACK_TRUE				1
#define MID_TURN_ON_LOOPBACK_CMD		0x84
#define MID_TURN_OFF_LOOPBACK_CMD		0x85

#define MID_LEN_MAX_ERROR_STRING		500

#define MID_CHK_IN_BUF_OK(size)	{if (size > MID_MAX_IN_BUF_SIZE) \
	{ return FT_INSUFFICIENT_RESOURCES;}}

// FT_STATUS FT_GetNumChannels(FT_LegacyProtocol Protocol, DWORD *numChans);
// FT_STATUS FT_GetChannelInfo(FT_LegacyProtocol Protocol, DWORD index,
// 			FT_DEVICE_LIST_INFO_NODE *chanInfo);
// FT_STATUS FT_OpenChannel(FT_LegacyProtocol Protocol, DWORD index,
// 			FT_HANDLE *handle);
// 
// 
// FT_STATUS FT_InitChannel(		
// 	FT_LegacyProtocol Protocol,	
// 	FT_HANDLE handle,			
// 	uint32 clockRate,			
// 	uint32 latencyTimer,		
// 	uint32 configOptions,		
// 	DWORD Pin);					
// 
// FT_STATUS FT_CloseChannel(FT_LegacyProtocol Protocol, FT_HANDLE handle);
// FT_STATUS FT_Channel_Read(FT_LegacyProtocol Protocol, FT_HANDLE handle,
// 				DWORD noOfBytes, uint8* buffer, LPDWORD noOfBytesTransferred);
// FT_STATUS FT_Channel_Write(FT_LegacyProtocol Protocol, FT_HANDLE handle,
// 			DWORD noOfBytes, uint8* buffer, LPDWORD noOfBytesTransferred);
// extern bool Mid_CheckMPSSEAvailable(FT_DEVICE_LIST_INFO_NODE);
// 
// extern FT_STATUS Mid_ResetDevice(FT_HANDLE handle);
// extern FT_STATUS Mid_PurgeDevice (FT_HANDLE handle);
// extern FT_STATUS Mid_SetUSBParameters(FT_HANDLE handle, DWORD inputBufSize,
// 										DWORD outputBufSize);
// extern FT_STATUS Mid_SetDeviceSpecialChar(FT_HANDLE handle,
// 			UCHAR eventCh, UCHAR eventStatus,
// 			UCHAR errorCh, UCHAR errorStatus);
// extern FT_STATUS Mid_SetDeviceTimeOut(FT_HANDLE handle,
// 										DWORD rdTimeOut, DWORD wrTimeOut);
// extern FT_STATUS Mid_SetLatencyTimer (FT_HANDLE handle, UCHAR milliSecond);
// extern FT_STATUS Mid_ResetMPSSE(FT_HANDLE handle);
// extern FT_STATUS Mid_EnableMPSSEIn(FT_HANDLE handle);
// extern FT_STATUS Mid_SyncMPSSE(FT_HANDLE handle);
// extern FT_STATUS Mid_SendReceiveCmdFromMPSSE(FT_HANDLE handle, 
// 			UCHAR echoCmdFlag, UCHAR ecoCmd, UCHAR *cmdEchoed);
// extern FT_STATUS Mid_SetGPIOLow(FT_HANDLE handle, uint8 value, uint8 direction);
// extern FT_STATUS Mid_SetClock(FT_HANDLE handle, FT_DEVICE ftDevice, uint32 clock);
// extern FT_STATUS Mid_GetFtDeviceType(FT_HANDLE handle, FT_DEVICE *ftDevice);
// extern FT_STATUS Mid_SetDeviceLoopbackState(FT_HANDLE handle, uint8 loopBackFlag);
// extern FT_STATUS Mid_EmptyDeviceInputBuff(FT_HANDLE handle);


}

