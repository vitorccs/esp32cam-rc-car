#include <Arduino.h>
#include <SocketServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

SocketServer::SocketServer() : webSocket(82)
{
    ALIAS_BUTTON_A = "button-a";
    ALIAS_DIRECTION = "direction";
    ALIAS_SPEED = "speed";

    auto handler = [&](uint8_t num,
                       WStype_t type,
                       uint8_t *payload,
                       size_t length)
    {
        return onEvent(num, type, payload, length);
    };

    webSocket.onEvent(handler);
}

void SocketServer::onEvent(uint8_t num,
                           WStype_t type,
                           uint8_t *payload,
                           size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
        Serial.printf("[%u] Connected!\n", num);
        break;
    case WStype_TEXT:
        Serial.print("Data received: ");
        Serial.println((char *)payload);
        break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void SocketServer::init(CoordsHandlerFunction coordsHandler,
                        ButtonAToggleHandlerFunction buttonAHandler)
{
    _coordsHandler = coordsHandler;
    _buttonAHandler = buttonAHandler;

    webSocket.begin();
}

void SocketServer::handleWebSocketMessage(char *dataChar) {

    const bool isMovCoords = strstr(dataChar, ALIAS_SPEED) != nullptr;
    const bool isMovCoords2 = strstr(dataChar, ALIAS_DIRECTION) != nullptr;
    const bool isButtonA = strstr(dataChar, ALIAS_BUTTON_A) != nullptr;

    if (isMovCoords && isMovCoords2)
    {
        // parse string as a two-members JSON
        const int jsonSize = JSON_OBJECT_SIZE(2);
        StaticJsonDocument<jsonSize> json;
        DeserializationError error = deserializeJson(json, dataChar);

        if (error)
        {
            return;
        }

        JoyCoords coords;
        coords.speed = json[ALIAS_SPEED];
        // coords.direction = String(json[ALIAS_DIRECTION]);
        coords.direction = "N";
        Serial.println("Aqui");

        _coordsHandler(coords);

        return;
    }

    if (isButtonA)
    {
        const int jsonSize = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<jsonSize> json;
        DeserializationError error = deserializeJson(json, dataChar);

        if (error)
        {
            return;
        }

        const bool enabled = json[ALIAS_BUTTON_A];

        _buttonAHandler(enabled);

        return;
    }
    
}

void SocketServer::loop()
{
    webSocket.loop();
}