#include "MY_QR.h"

int get_max_ecc_level(String qrData, int version, int ecc_version=0)
{
  int length = qrData.length()*8;
  
  int maxLenLow[40] = {152, 272, 440,  640, 864, 1088, 1248, 1552, 1856, 2192, 2592, 2960, 3424, 3688, 4184, 4712, 5176, 5768, 6360, 6888};
  int maxLenMedium[40] = {128, 224, 352, 512, 688, 864, 992, 1232, 1456, 1728, 2032, 2320, 2672, 2920, 3320, 3624, 4056, 4504, 5016, 5352};
  int maxLenQuartile[40] = {104, 176, 272, 384, 496, 608, 704, 880, 1056, 1232, 1440, 1648, 1952, 2088, 2360, 2600, 2936, 3176, 3560, 3880};
  int maxLenHigh[40] = {72, 128, 208, 288, 368, 480, 528, 688, 800, 976, 1120, 1264, 1440, 1576, 1784, 2024, 2264, 2504, 2728, 3080};

  switch (ecc_version)
  {
  case 0:
    if (length < maxLenHigh[version - 1])
    {
      return ECC_HIGH;
    }
  case 1:
    if (length < maxLenQuartile[version - 1])
    {
      return ECC_QUARTILE;
    }
  case 2:
    if (length < maxLenMedium[version - 1])
    {
      return ECC_MEDIUM;
    }
  case 3:
    if (length < maxLenLow[version - 1])
    {
      return ECC_LOW;
    }
  default:
      return -1;
  }
}

QRCodeInfo MyQr::get_qrcode_version(String qrData, DisplayInfo &DIdata)
{
  int length = qrData.length();
  int min_size = DIdata.DX - DIdata.DOX;
  if (DIdata.DY < DIdata.DX)
  {
    min_size = DIdata.DYDIdata.DOY;
  }
  int max_version = DIdata.MMV;
  if (DIdata.AMV){
    max_version = (min_size-17)/4;
  }
  for (int version = 1; version <= max_version; version++)
  {
    int ecc = get_max_ecc_level(qrData, version);
    if (ecc != -1)
    {
      return {version, ecc};
    }
  }
  return {-1, -1};
}

bool MyQr::create_qr(String arData, Adafruit_SSD1306 &display, DisplayInfo &data)
{
  QRCodeInfo result = get_qrcode_version(qrData);
  if (version < 0)
  {
      return false;
  }

  QRCode qrcode;
  uint8_t qrcodeData[ (( 4*result.version + 7 ) * ( 4*result.version + 7 )) / 8 ];
  qrcode_initText(&qrcode, qrcodeData, result.version, result.eccLevel, qrData.c_str());

  if (data.AS)
  {
      data.MS = qrcode.size/(Y-data.DOY);
      if (data.MS < 1)
      {
          data.MS = 1;
      }
  }

  for (uint8_t y = 0; y < qrcode.size; y++)
  {
      for (uint8_t x = 0; x < qrcode.size; x++)
      {
          if (qrcode_getModule(&qrcode, x, y))
          {
              display.fillRect((x*data.MS) + data.DOX, (y*data.MS) + data.DOY, data.MS, data.MS, SSD1306_WHITE);
          }
      }
  }

  display.display();
  return true;
}