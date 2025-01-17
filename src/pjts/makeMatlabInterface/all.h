#include <windows.h>
#include <assert.h>	
#include "string.h"


#define FTDIMPSSE_API __declspec(dllexport)
#define WINAPI      __stdcall
typedef ULONG	FT_STATUS;


FTDIMPSSE_API void WINAPI Init_libMPSSE(void);
FTDIMPSSE_API void WINAPI Cleanup_libMPSSE(void);
FTDIMPSSE_API FT_STATUS WINAPI Ver_libMPSSE(LPDWORD libmpsse, LPDWORD libftd2xx);
