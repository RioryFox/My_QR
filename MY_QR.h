#include <qrcode.h>

struct DisplayInfo
{
    int displayY=64;
    int displayX=128;
    int topLeftY=0;
    int topLeftX=0;
    int myScale=1;
    bool autoScale=false;
    int myMaxVersion=1;
    bool autoMaxVersion=true;
};

struct QRCodeInfo
{
  int version=-1;
  int eccLevel=-1;
};

class MyQr
{
    public :
        QRCodeInfo get_qrcode_version(String qrData, DisplayInfo &DIdata);
        bool create_qr(String qrData, Adafruit_SSD1306 &display, DisplayInfo &data);
    
    private:
        int get_max_ecc_level(String qrData, int version, int ecc_version=0);
};