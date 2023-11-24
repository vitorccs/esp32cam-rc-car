#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H
#include <Arduino.h>
#include <WiFi.h>
#include <WifiHandler.h>

void WifiHandler::connect(const char *ssid,
                          const char *pwd)
{
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    debug(WiFi, ssid, false);
}

void WifiHandler::apMode(const char *ssid,
                         const char *pwd,
                         int channel)
{
    WiFi.softAP(ssid, pwd, channel);

    debug(WiFi, ssid, true);
}

void WifiHandler::debug(WiFiClass WiFi,
                        const char *ssid,
                        bool apMode)
{
    Serial.println("");

    if (apMode)
    {
        Serial.print("AP Mode connected at ");
    }
    else
    {
        Serial.print("Wi-Fi connected at ");
    }

    Serial.println(ssid);
    Serial.print("http://");

    if (apMode)
    {
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println(WiFi.localIP());
    }
}

#endif