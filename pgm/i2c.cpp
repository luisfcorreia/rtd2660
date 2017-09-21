#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "i2c.h"
#include "i2c-dev.h"

// I2C Linux device handle
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
    printf("Working with device %02x\n",address);

    if (ioctl(g_i2cFile, I2C_SLAVE, address) < 0)
    {
        perror("i2cSetAddress");
        exit(1);
    }
}

bool WriteBytesToAddr(uint8_t reg, uint8_t* values, uint8_t len)
{
    uint8_t buf[64];
    if (len > 63)
    {
        len = 63;
    }
    uint8_t buflen = len + 1;
    buf[0] = reg;

    for(int idx = 1; idx < buflen; idx++)
    {
        buf[idx] = values[idx-1];
    }
    /*
        for(int idx = 0; idx < buflen; idx++)
        {
            printf("buf[%i] = %02x ",idx,buf[idx]);
        }
        printf("\n");
    */
    if (write(g_i2cFile, buf, buflen) != buflen)
    {
        printf("Failed to write to the i2c bus\n");
        return 0;
    }
    else
    {
        return 1;
    }
}

bool ReadBytesFromAddr(uint8_t reg, uint8_t* dest, uint8_t len)
{
    //read() returns the number of bytes actually read, if it doesn't match
    //then an error occurred (e.g. no response from the device)
    if (read(g_i2cFile, dest, len) != len)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t ReadReg(uint8_t reg)
{
/*
    uint8_t res;

    res = i2c_smbus_write_byte(g_i2cFile, reg);
    if (res < 0)
        printf("Warning - write failed\n");
*/
    return i2c_smbus_read_byte_data(g_i2cFile,reg);
    /*

        uint8_t result;
        memset(&result,0,1);

        if(ReadBytesFromAddr(reg, &result, 1))
        {
            printf("Read %02x from %02x\n",result,reg);
        }
        else
        {
            result =0;
            printf("Failed to read from the i2c bus\n");
        };

        return result;
        */
}

bool WriteReg(uint8_t reg, uint8_t value)
{
    //printf("Writing %02x to %02x\n",value,reg);
    return WriteBytesToAddr(reg, &value, 1);
}
