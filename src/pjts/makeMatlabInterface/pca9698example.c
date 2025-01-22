#include <stdio.h>
#include <stdint.h>
#include "ftdi_i2c.h" // FTDI I2C 라이브러리 헤더 파일

#define PCA9698_I2C_ADDRESS 0x20 // PCA9698의 I2C 주소

// PCA9698 레지스터 주소
#define PCA9698_REG_INPUT_PORT0 0x00
#define PCA9698_REG_OUTPUT_PORT0 0x08
#define PCA9698_REG_CONFIG_PORT0 0x18

// I2C 핸들
FT_HANDLE handle;

// I2C 초기화 함수
FT_STATUS I2C_Init()
{
    FT_STATUS status;
    DWORD numChannels;

    // I2C 채널 수 가져오기
    status = I2C_GetNumChannels(&numChannels);
    if (status != FT_OK || numChannels == 0)
    {
        printf("I2C 채널을 찾을 수 없습니다.\n");
        return status;
    }

    // 첫 번째 I2C 채널 열기
    status = I2C_OpenChannel(0, &handle);
    if (status != FT_OK)
    {
        printf("I2C 채널을 열 수 없습니다.\n");
        return status;
    }

    // I2C 채널 초기화
    I2C_ChannelConfig config;
    config.ClockRate = I2C_CLOCK_STANDARD_MODE;
    config.LatencyTimer = 16;
    config.Options = 0;
    status = I2C_InitChannel(handle, &config);
    if (status != FT_OK)
    {
        printf("I2C 채널을 초기화할 수 없습니다.\n");
        return status;
    }

    return FT_OK;
}

// PCA9698 레지스터 쓰기 함수
FT_STATUS PCA9698_WriteRegister(uint8_t reg, uint8_t value)
{
    FT_STATUS status;
    uint8_t buffer[2];
    DWORD bytesTransferred;

    buffer[0] = reg;
    buffer[1] = value;

    status = I2C_DeviceWrite(handle, PCA9698_I2C_ADDRESS, 2, buffer, &bytesTransferred, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    if (status != FT_OK)
    {
        printf("레지스터 쓰기 실패: 0x%02X\n", reg);
    }

    return status;
}

// PCA9698 레지스터 읽기 함수
FT_STATUS PCA9698_ReadRegister(uint8_t reg, uint8_t *value)
{
    FT_STATUS status;
    DWORD bytesTransferred;

    status = I2C_DeviceWrite(handle, PCA9698_I2C_ADDRESS, 1, &reg, &bytesTransferred, I2C_TRANSFER_OPTIONS_START_BIT);
    if (status != FT_OK)
    {
        printf("레지스터 주소 쓰기 실패: 0x%02X\n", reg);
        return status;
    }

    status = I2C_DeviceRead(handle, PCA9698_I2C_ADDRESS, 1, value, &bytesTransferred, I2C_TRANSFER_OPTIONS_STOP_BIT);
    if (status != FT_OK)
    {
        printf("레지스터 읽기 실패: 0x%02X\n", reg);
    }

    return status;
}

int main()
{
    FT_STATUS status;

    // I2C 초기화
    status = I2C_Init();
    if (status != FT_OK)
    {
        return -1;
    }

    // PCA9698 포트 0을 출력으로 설정
    status = PCA9698_WriteRegister(PCA9698_REG_CONFIG_PORT0, 0x00);
    if (status != FT_OK)
    {
        return -1;
    }

    // PCA9698 포트 0에 값 쓰기
    status = PCA9698_WriteRegister(PCA9698_REG_OUTPUT_PORT0, 0xFF);
    if (status != FT_OK)
    {
        return -1;
    }

    // PCA9698 포트 0에서 값 읽기
    uint8_t value;
    status = PCA9698_ReadRegister(PCA9698_REG_INPUT_PORT0, &value);
    if (status != FT_OK)
    {
        return -1;
    }

    printf("PCA9698 포트 0 값: 0x%02X\n", value);

    // I2C 채널 닫기
    I2C_CloseChannel(handle);

    return 0;
}
