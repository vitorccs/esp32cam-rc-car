#ifndef STREAMSERVER_H
#define STREAMSERVER_H
#include <Arduino.h>
#include <esp_http_server.h>
#include <sensor.h>

class StreamServer
{
public:
    void init(framesize_t frameSize, int jpegQuality);
    void startStream();
    static esp_err_t index_handler(httpd_req_t *req);
    static esp_err_t stream_handler(httpd_req_t *req);
};

#endif