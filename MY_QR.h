#ifndef MY_QR_H
#define MY_QR_H

#include <qrcode.h>

struct DisplayInfo
{
    int DY;
    int DX;
    int DOY;
    int DOX;
    int MS;
    bool AS;
    int MMV;
    bool AMV;
};

struct QRCodeInfo
{
  int version;
  int eccLevel;
};

class MyQr
{
    public :
        QRCodeInfo get_qrcode_version(String qrData, DisplayInfo &DIdata);
        bool create_qr(String qrData, Adafruit_SSD1306 &display, DisplayInfo &data);
    
    private:
        int get_max_ecc_level(String qrData, int version, int ecc_version=0);
};

#endif