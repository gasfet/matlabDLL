#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FTDIMPSSE_API __declspec(dllexport)
#define WINAPI      __stdcall

typedef ULONG    FT_STATUS;
typedef PVOID    FT_HANDLE;

typedef struct _ft_device_list_info_node {
    ULONG Flags;
    ULONG Type;
    ULONG ID;
    DWORD LocId;
    char SerialNumber[16];
    char Description[64];
    FT_HANDLE ftHandle;
} FT_DEVICE_LIST_INFO_NODE;

typedef enum I2C_ClockRate_t{
    I2C_CLOCK_STANDARD_MODE = 100000,        /* 100kb/sec */
    I2C_CLOCK_FAST_MODE = 400000,            /* 400kb/sec */
    I2C_CLOCK_FAST_MODE_PLUS = 1000000,        /* 1000kb/sec */
    I2C_CLOCK_HIGH_SPEED_MODE = 3400000        /* 3.4Mb/sec */
} I2C_CLOCKRATE;

/**/
typedef struct ChannelConfig_i2c
{
    I2C_CLOCKRATE    ClockRate;
    /** There were 2 functions I2C_TurnOn/OffDivideByFive
    ClockinghiSpeedDevice (FTC_HANDLE fthandle) in the old DLL. This function turns on the
    divide by five for the MPSSE clock to allow the hi-speed devices FT2232H and FT4232H to
    clock at the same rate as the FT2232D device. This allows for backward compatibility
    NOTE: This feature is probably a per chip feature and not per device*/

    UCHAR            LatencyTimer;
    /** Required value, in milliseconds, of latency timer.
    Valid range is 2 ~ 255
    In the FT8U232AM and FT8U245AM devices, the receive buffer timeout that is used to flush
    remaining data from the receive buffer was fixed at 16 ms. In all other FTDI devices, this
    timeout is programmable and can be set at 1 ms intervals between 2ms and 255 ms.  This
    allows the device to be better optimized for protocols requiring faster response times from
    short data packets
    NOTE: This feature is probably a per chip feature and not per device*/

    DWORD            Options;
    /** This member provides a way to enable/disable features
    specific to the protocol that are implemented in the chip
    BIT0        : 3PhaseDataClocking - Setting this bit will turn on 3 phase data clocking for a
            FT2232H dual hi-speed device or FT4232H quad hi-speed device. Three phase
            data clocking, ensures the data is valid on both edges of a clock
    BIT1        : Loopback
    BIT2        : Clock stretching
    BIT3         : Enable PinState config
    BIT4 - BIT31        : Reserved
    */

    DWORD        Pin;/* BIT7   -BIT0:   Initial direction of the pins    */
                    /* BIT15 -BIT8:   Initial values of the pins        */
                    /* BIT23 -BIT16: Final direction of the pins        */
                    /* BIT31 -BIT24: Final values of the pins        */

    USHORT        currentPinState;/* BIT7   -BIT0:   Current direction of the pins    */
                                /* BIT15 -BIT8:   Current values of the pins    */
} I2C_ChannelConfig;

/* This structure associates the channel configuration information to a handle stores them in the
form of a linked list */
typedef struct ChannelContext_i2c
{
    FT_HANDLE         handle;
    I2C_ChannelConfig    config;
    struct ChannelContext_i2c *next;
} I2C_ChannelContext;

FTDIMPSSE_API void WINAPI Init_libMPSSE(void);
FTDIMPSSE_API void WINAPI Cleanup_libMPSSE(void);
FTDIMPSSE_API FT_STATUS WINAPI Ver_libMPSSE(LPDWORD libmpsse, LPDWORD libftd2xx);

FTDIMPSSE_API FT_STATUS I2C_GetNumChannels(DWORD *numChannels);
FTDIMPSSE_API FT_STATUS I2C_CloseChannel(FT_HANDLE handle);
FTDIMPSSE_API FT_STATUS I2C_DeviceRead(FT_HANDLE handle, UCHAR deviceAddress,
    DWORD sizeToTransfer, UCHAR *buffer, LPDWORD sizeTransfered, DWORD options);
FTDIMPSSE_API FT_STATUS I2C_DeviceWrite(FT_HANDLE handle, UCHAR deviceAddress,
    DWORD sizeToTransfer, UCHAR *buffer, LPDWORD sizeTransfered, DWORD options);
FTDIMPSSE_API FT_STATUS I2C_GetChannelInfo(DWORD index,
    FT_DEVICE_LIST_INFO_NODE *chanInfo);
FTDIMPSSE_API FT_STATUS I2C_GetDeviceID(FT_HANDLE handle, UCHAR deviceAddress,
    UCHAR *deviceID);
FTDIMPSSE_API FT_STATUS I2C_GetNumChannels(DWORD *numChannels);
FTDIMPSSE_API FT_STATUS I2C_InitChannel(FT_HANDLE handle, I2C_ChannelConfig *config);
FTDIMPSSE_API FT_STATUS I2C_OpenChannel(DWORD index, FT_HANDLE *handle);
