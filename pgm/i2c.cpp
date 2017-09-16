#include <stdint.h>
#include "types.h"
//#include <Windows.h>
//#include "CyAPI.h"
//#include "cyioctl.h"

//static CCyUSBDevice USBDevice;
//static CCyControlEndPoint *ept = USBDevice.ControlEndPt;
/*
static BYTE ReadNakCnt()
{
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_FROM_DEVICE;
  ept->ReqCode = 0xA3;
  ept->Value = 0;
  ept->Index = 0;

  uint8_t buf[1];
  LONG buflen = sizeof(buf);
  ept->XferData(buf, buflen);
  return buf[0];
}

void SetI2CAddr(uint8_t value)
{
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xA5;
  ept->Value = 0;
  ept->Index = 0;

  uint8_t buf[1];
  LONG buflen =  1;
  buf[0] = value;
  ept->XferData(buf, buflen);
}

bool WriteBytesToAddr(uint8_t reg, uint8_t* values, uint8_t len)
{
  ept->Target    = TGT_DEVICE;
  ept->ReqType   = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode   = 0xA2;
  ept->Value     = 0;
  ept->Index     = 0;

  uint8_t buf[64];
  if (len > 63)
  {
    len = 63;
  }
  LONG buflen =  len + 1;
  buf[0] = reg;
  for(int idx = 0; idx < len; idx++)
  {
    buf[1 + idx] = values[idx];
  }

  ept->XferData(buf, buflen);
  return ReadNakCnt() == 0;
}

static void ReadBytes(uint8_t *dest, uint8_t len)
{
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_FROM_DEVICE;
  ept->ReqCode = 0xA4;
  ept->Value = 0;
  ept->Index = 0;

  uint8_t buf[64];
  LONG buflen = sizeof(buf);
  ept->XferData(buf, buflen);
  if (len > sizeof(buf))
  {
    len = (uint8_t)sizeof(buf);
  }
  memcpy(dest, buf, len);
}

bool ReadBytesFromAddr(uint8_t reg, uint8_t* dest, uint8_t len)
{
  ept->Target    = TGT_DEVICE;
  ept->ReqType   = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode   = 0xA1;
  ept->Value     = 0;
  ept->Index     = 0;

  uint8_t buf[2];
  LONG buflen = sizeof(buf);
  buf[0] = reg;
  buf[1] = len;
  ept->XferData(buf, buflen);
  ReadBytes(dest, len);
  return ReadNakCnt() == 0;
}

uint8_t ReadReg(uint8_t reg)
{
  uint8_t result;
  ReadBytesFromAddr(reg, &result, 1);
  return result;
}

bool WriteReg(uint8_t reg, uint8_t value)
{
  return WriteBytesToAddr(reg, &value, 1);
}

bool InitI2C() {
  return USBDevice.IsOpen();
}

void CloseI2C() {
  USBDevice.Close();
}
*/

bool WriteBytesToAddr(uint8_t reg, uint8_t* values, uint8_t len)
{

}

bool WriteReg(uint8_t reg, uint8_t value)
{
}

bool ReadBytesFromAddr(uint8_t reg, uint8_t* dest, uint8_t len)
{
}

uint8_t ReadReg(uint8_t reg)
{
}

void SetI2CAddr(uint8_t value)
{
}

bool InitI2C() {
return 1;
}


void CloseI2C() {
}
