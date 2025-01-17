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

typedef struct ChannelConfig_i2c
{
    I2C_CLOCKRATE    ClockRate;
    UCHAR            LatencyTimer;
    DWORD            Options;
    DWORD        Pin;
    USHORT        currentPinState;
} I2C_ChannelConfig;


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
FTDIMPSSE_API FT_STATUS I2C_GetChannelInfo(DWORD index, FT_DEVICE_LIST_INFO_NODE *chanInfo);
FTDIMPSSE_API FT_STATUS I2C_OpenChannel(DWORD index, FT_HANDLE *handle);

FTDIMPSSE_API FT_STATUS I2C_InitChannel(FT_HANDLE handle, I2C_ChannelConfig *config);
FTDIMPSSE_API FT_STATUS I2C_CloseChannel(FT_HANDLE handle);

FTDIMPSSE_API FT_STATUS I2C_DeviceRead(FT_HANDLE handle, UCHAR deviceAddress, DWORD sizeToTransfer, UCHAR *buffer, LPDWORD sizeTransfered, DWORD options);
FTDIMPSSE_API FT_STATUS I2C_DeviceWrite(FT_HANDLE handle, UCHAR deviceAddress, DWORD sizeToTransfer, UCHAR *buffer, LPDWORD sizeTransfered, DWORD options);
FTDIMPSSE_API FT_STATUS I2C_GetDeviceID(FT_HANDLE handle, UCHAR deviceAddress, UCHAR *deviceID);

typedef struct ChannelConfig_spi
{
	DWORD	ClockRate; 
	UCHAR	LatencyTimer;
	DWORD	Options;	
	DWORD		Pin;
	USHORT		currentPinState;
}SPI_ChannelConfig;

typedef struct ChannelContext_spi
{
	FT_HANDLE 		handle;
	SPI_ChannelConfig	config;
	struct ChannelContext_spi *next;
}SPI_ChannelContext;

FTDIMPSSE_API FT_STATUS SPI_GetNumChannels(DWORD *numChannels);
FTDIMPSSE_API FT_STATUS SPI_GetChannelInfo(DWORD index,    FT_DEVICE_LIST_INFO_NODE *chanInfo);
FTDIMPSSE_API FT_STATUS SPI_OpenChannel(DWORD index, FT_HANDLE *handle);

FTDIMPSSE_API FT_STATUS SPI_InitChannel(FT_HANDLE handle, SPI_ChannelConfig *config);
FTDIMPSSE_API FT_STATUS SPI_CloseChannel(FT_HANDLE handle);
FTDIMPSSE_API FT_STATUS SPI_IsBusy(FT_HANDLE handle, BOOL *state);
FTDIMPSSE_API FT_STATUS SPI_Read(FT_HANDLE handle, UCHAR *buffer, DWORD sizeToTransfer, LPDWORD sizeTransfered, DWORD options);
FTDIMPSSE_API FT_STATUS SPI_Write(FT_HANDLE handle, UCHAR *buffer,    DWORD sizeToTransfer, LPDWORD sizeTransfered, DWORD options);
FTDIMPSSE_API FT_STATUS SPI_ReadWrite(FT_HANDLE handle, UCHAR *inBuffer, UCHAR *outBuffer, DWORD sizeToTransfer, LPDWORD sizeTransferred, DWORD transferOptions);
FTDIMPSSE_API FT_STATUS SPI_ChangeCS(FT_HANDLE handle, DWORD configOptions);
FTDIMPSSE_API FT_STATUS SPI_ToggleCS(FT_HANDLE handle, BOOL state);
