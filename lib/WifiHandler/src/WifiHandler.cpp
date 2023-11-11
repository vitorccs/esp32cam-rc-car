#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H
#include <Arduino.h>
#include <WiFi.h>
#include <WifiHandler.h>

void WifiHandler::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("WiFi connected at ");
    Serial.println(ssid);

    Serial.print("http://");
    Serial.println(WiFi.localIP());
}

#endif