#ifndef FTDI_I2C_H
#define FTDI_I2C_H

typedef void * PVOID;
typedef PVOID	FT_HANDLE;

#ifdef __cplusplus
extern "C" {
#endif

#include "ftdi_infra.h"		/*Common portable infrastructure(datatypes, libraries, etc)*/
#include "ftdi_common.h"	/*Common across I2C, SPI, JTAG modules*/
#include "ftdi_mid.h"		/*Middle layer*/

#ifndef FTDIMPSSE_API
#ifdef _WIN32
// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the FTDIMPSSE_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
// FTDIMPSSE_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef FTDIMPSSE_EXPORTS
#define FTDIMPSSE_API __declspec(dllexport)
#elif defined(FTDIMPSSE_STATIC)
// Avoid decorations when linking statically.
#define FTDIMPSSE_API
#else // FTDIMPSSE_EXPORTS
#define FTDIMPSSE_API __declspec(dllimport)
#endif // FTDIMPSSE_EXPORTS

#else // _WIN32

// Compiling on non-Windows platform.
#include "WinTypes.h"
// No decorations needed.
#define FTDIMPSSE_API

#endif // _WIN32
#endif // FTDIMPSSE_API

/******************************************************************************/
/*								Macro defines								  */
/******************************************************************************/

/** Options to I2C_DeviceWrite & I2C_DeviceRead */
/*Generate start condition before transmitting. */
#define	I2C_TRANSFER_OPTIONS_START_BIT		0x00000001

/*Generate stop condition before transmitting. */
#define I2C_TRANSFER_OPTIONS_STOP_BIT		0x00000002

/*Continue transmitting data in bulk without caring about Ack or nAck from device if this bit
is not set. If this bit is set then stop transferring the data in the buffer when the device
nACKs. */
#define I2C_TRANSFER_OPTIONS_BREAK_ON_NACK	0x00000004

/* libMPSSE-I2C generates an ACKs for every byte read. Some I2C slaves require the I2C
master to generate a nACK for the last data byte read. Setting this bit enables working with
such I2C slaves. */
#define I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE	0x00000008

/*Fast transfers prepare a buffer containing commands to generate START/STOP/ADDRESS
   conditions and commands to read/write data. This buffer is sent to the MPSSE in one shot,
   hence delays between different phases of the I2C transfer are eliminated. Fast transfers
   can have data length in terms of bits or bytes. The user application should call
   I2C_DeviceWrite or I2C_DeviceRead with either
   I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES or
   I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BITS bit set to perform a fast transfer.
   I2C_TRANSFER_OPTIONS_START_BIT and I2C_TRANSFER_OPTIONS_STOP_BIT have
   their usual meanings when used in fast transfers, however
   I2C_TRANSFER_OPTIONS_BREAK_ON_NACK and
   I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE are not applicable in fast transfers. */
#define I2C_TRANSFER_OPTIONS_FAST_TRANSFER		0x00000030/*not visible to user*/

/* When the user calls I2C_DeviceWrite or I2C_DeviceRead with this bit set then libMPSSE
	 packs commands to transfer sizeToTransfer number of bytes, and to read/write
	 sizeToTransfer number of ack bits. If data is written then the read ack bits are ignored, if
	 data is being read then an acknowledgement bit(SDA=LOW) is given to the I2C slave
	 after each byte read. */
#define I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES	0x00000010

/* When the user calls I2C_DeviceWrite or I2C_DeviceRead with this bit set then libMPSSE
	 packs commands to transfer sizeToTransfer number of bits. There is no ACK phase when
	 this bit is set. */
#define I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BITS	0x00000020

/* The address parameter is ignored in transfers if this bit is set. This would mean that
	 the address is either a part of the data or this is a special I2C frame that doesn't require
	 an address. However if this bit is not set then 7bit address and 1bit direction will be
	 written to the I2C bus each time I2C_DeviceWrite or I2C_DeviceRead is called and a
	 1bit acknowledgement will be read after that. */
#define I2C_TRANSFER_OPTIONS_NO_ADDRESS		0x00000040

#define I2C_CMD_GETDEVICEID_RD	0xF9
#define I2C_CMD_GETDEVICEID_WR	0xF8

#define I2C_GIVE_ACK	1
#define I2C_GIVE_NACK	0

/* 3-phase clocking is enabled by default. Setting this bit in ConfigOptions will disable it */
#define I2C_DISABLE_3PHASE_CLOCKING	0x0001

/* option to enable pinstate configuration */
#define I2C_ENABLE_PIN_STATE_CONFIG 0x0010

/******************************************************************************/
/*								Type defines								  */
/******************************************************************************/

/**
Valid range for clock divisor is 0 to 65535
Highest clock freq is 6MHz represented by 0
The next highest is   3MHz represented by 1
Lowest is            91Hz  represented by 65535

User can pass either pass I2C_DataRate_100K, I2C_DataRate_400K or
I2C_DataRate_3_4M for the standard clock rates
or a clock divisor value may be passed
*/
typedef enum I2C_ClockRate_t{
	I2C_CLOCK_STANDARD_MODE = 100000,		/* 100kb/sec */
	I2C_CLOCK_FAST_MODE = 400000,			/* 400kb/sec */
	I2C_CLOCK_FAST_MODE_PLUS = 1000000,		/* 1000kb/sec */
	I2C_CLOCK_HIGH_SPEED_MODE = 3400000		/* 3.4Mb/sec */
} I2C_CLOCKRATE;

/**/
typedef struct ChannelConfig_i2c
{
	I2C_CLOCKRATE	ClockRate; 
	/** There were 2 functions I2C_TurnOn/OffDivideByFive
	ClockinghiSpeedDevice (FTC_HANDLE fthandle) in the old DLL. This function turns on the
	divide by five for the MPSSE clock to allow the hi-speed devices FT2232H and FT4232H to
	clock at the same rate as the FT2232D device. This allows for backward compatibility
	NOTE: This feature is probably a per chip feature and not per device*/

	UCHAR			LatencyTimer; 
	/** Required value, in milliseconds, of latency timer.
	Valid range is 2 � 255
	In the FT8U232AM and FT8U245AM devices, the receive buffer timeout that is used to flush
	remaining data from the receive buffer was fixed at 16 ms. In all other FTDI devices, this
	timeout is programmable and can be set at 1 ms intervals between 2ms and 255 ms.  This
	allows the device to be better optimized for protocols requiring faster response times from
	short data packets
	NOTE: This feature is probably a per chip feature and not per device*/

	DWORD			Options;	
	/** This member provides a way to enable/disable features
	specific to the protocol that are implemented in the chip
	BIT0		: 3PhaseDataClocking - Setting this bit will turn on 3 phase data clocking for a
			FT2232H dual hi-speed device or FT4232H quad hi-speed device. Three phase
			data clocking, ensures the data is valid on both edges of a clock
	BIT1		: Loopback
	BIT2		: Clock stretching
	BIT3 		: Enable PinState config
	BIT4 - BIT31		: Reserved
	*/

	DWORD		Pin;/* BIT7   -BIT0:   Initial direction of the pins	*/
					/* BIT15 -BIT8:   Initial values of the pins		*/
					/* BIT23 -BIT16: Final direction of the pins		*/
					/* BIT31 -BIT24: Final values of the pins		*/

	USHORT		currentPinState;/* BIT7   -BIT0:   Current direction of the pins	*/
								/* BIT15 -BIT8:   Current values of the pins	*/
} I2C_ChannelConfig;


typedef struct ChannelContext_i2c
{
	FT_HANDLE 		handle;
	I2C_ChannelConfig	config;
	struct ChannelContext_i2c *next;
} I2C_ChannelContext;

#ifdef __cplusplus
}
#endif

#endif	/*FTDI_I2C_H*/

