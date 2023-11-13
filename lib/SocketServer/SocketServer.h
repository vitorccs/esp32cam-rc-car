#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <JoyCoords.h>

typedef std::function<void(JoyCoords coords)> CoordsHandlerFunction;
typedef std::function<void(bool buttonToggle)> ButtonToggleHandlerFunction;

class SocketServer
{
public:
    SocketServer();

    void init(CoordsHandlerFunction coordsHandler,
              ButtonToggleHandlerFunction buttonAHandler,
              ButtonToggleHandlerFunction buttonBHandler);

    void onEvent(uint8_t num,
                 WStype_t type,
                 uint8_t *payload,
                 size_t length);

    void handleWebSocketMessage(char *dataChar);

    void loop();

private:
    WebSocketsServer webSocket;
    CoordsHandlerFunction _coordsHandler;
    ButtonToggleHandlerFunction _buttonAHandler;
    ButtonToggleHandlerFunction _buttonBHandler;
    const char *ALIAS_DIRECTION;
    const char *ALIAS_SPEED;
    const char *ALIAS_BUTTON_A;
    const char *ALIAS_BUTTON_B;
};

#endif