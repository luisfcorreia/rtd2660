#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "i2c.h"

// I2C Linux device handle
int g_i2cFile;

// open the Linux device
void InitI2C()
{
    g_i2cFile = open("/dev/i2c-1", O_RDWR);
    if (g_i2cFile < 0)
    {
        perror("i2cOpen");
        exit(1);
    }
}

// close the Linux device
void CloseI2C()
{
    close(g_i2cFile);
}

void SetI2CAddr(uint8_t address)
{
    if (ioctl(g_i2cFile, I2C_SLAVE, address) < 0)
    {
        perror("i2cSetAddress");
        exit(1);
    }
}

static BYTE ReadNakCnt()
{
    uint8_t buf[1];
    LONG buflen = sizeof(buf);
    //ept->XferData(buf, buflen);
    return buf[0];
}

bool WriteBytesToAddr(uint8_t reg, uint8_t* values, uint8_t len)
{
    if (write(g_i2cFile, values, len) != len)
    {
        printf("Failed to write to the i2c bus.\n");
        return 0;
    }
    else
    {
        return 1;
    }
}

bool WriteReg(uint8_t reg, uint8_t value)
{
    return WriteBytesToAddr(reg, &value, 1);
}

bool ReadBytesFromAddr(uint8_t reg, uint8_t* dest, uint8_t len)
{
    if (read(g_i2cFile, dest, len) != len)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        //ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t ReadReg(uint8_t reg)
{
    uint8_t result;
    ReadBytesFromAddr(reg, &result, 1);
    return result;
}

