#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



#define FTDIMPSSE_API __declspec(dllexport)
#define WINAPI      __stdcall
typedef ULONG	FT_STATUS;

extern "C" {



FTDIMPSSE_API void WINAPI Init_libMPSSE(void);
FTDIMPSSE_API void WINAPI Cleanup_libMPSSE(void);
FTDIMPSSE_API FT_STATUS WINAPI Ver_libMPSSE(LPDWORD libmpsse, LPDWORD libftd2xx);


}