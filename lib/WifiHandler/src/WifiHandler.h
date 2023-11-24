#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H
#include <WiFi.h>

class WifiHandler
{
public:
    void connect(const char *ssid, const char *pwd);
    void apMode(const char *ssid, const char *pwd, int channel = 1);
    void debug(WiFiClass WiFi, const char *ssid, bool apMode = false);
};
#endif