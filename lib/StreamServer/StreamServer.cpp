#include <Arduino.h>
#include "esp_http_server.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h"           // disable brownout problems
#include "soc/rtc_cntl_reg.h"  // disable brownout problems
#include <StreamServer.h>

#define CAMERA_MODEL_AI_THINKER
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM_B
//#define CAMERA_MODEL_WROVER_KIT

#if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 25
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#elif defined(CAMERA_MODEL_M5STACK_WITHOUT_PSRAM)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 25
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 17
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#elif defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM_B)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 22
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#else
#error "Camera model not selected"
#endif

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
<title>JoyStick</title>
<style>
/* global */
*,
::after,
::before {
    box-sizing: content-box;
}

html,
body {
    padding: 0;
    margin: 0;
}

body {
    width: 100%;
    height: 100vh;
    overflow: hidden;
    font-family: 'Roboto', 'Tahoma', 'Arial', sans-serif;
    background: #6c8bee;
}

button {
    background: radial-gradient(ellipse at center, #444444 0%, #222222 100%);
    border: 1px solid #222;
    color: #fff;
    padding: 1rem clamp(1rem, 6vh, 3rem);
    font-size: clamp(1rem, 2.5vw, 2rem);
    border-radius: .4rem;
    margin: 1rem 0;
    user-select: none;
}

button.pressed {
    background: #444;
    box-shadow: inset 3px 1px 7px 4px #222;
}


/* debug */
.debug {
    position: absolute;
    background-color: rgba(0, 0, 0, .6);
    color: #ccc;
    bottom: 0;
    right: 0;
    margin: 0 auto;
    padding: .4rem .5rem;
}

/* container */
.container {
    display: flex;
    align-items: center;
    width: 100%;
    height: 100%;
}


/* container elements */

.container .video {
    background: #111;
    width: 100%;
}

.container .video img {
    width: 100%;
    min-width: 350px;
    aspect-ratio: 4 / 3;
}

.container .joystick {
    display: flex;
    justify-content: center;
    width: 100%;
    border: 0px solid red;
}

.container .buttons {
    display: flex;
    flex-direction: column;
    justify-content: end;
    padding: 0 3rem;
    border: 0px solid green;
}

.container .joystick #joystick-canvas {
    width: 80%;
    aspect-ratio: 1/1;
}

/* buttons container */
.buttons button.pressed:nth-child(1) {
    color: yellow;
}

.buttons button.pressed:nth-child(2) {
    color: blue;
}

@media (max-width: 800px) {
    .container {
        flex-direction: column;
        align-items: normal;
    }
    .container .video {
        order: 1;
    }
    .container .joystick {
        order: 2;
    }
    .container .buttons {
        order: 3;
        padding: 0 2rem;
    }
}

@media (min-width: 1000px) {
    .container .buttons {
        padding: 0 5rem;
    }
}
</style>
<script>
    /*
    * Name          : joy.js
    * @author       : Roberto D'Amico (Bobboteck)
    * Last modified : 09.06.2020
    * Revision      : 1.1.6
    * Source        : https://github.com/bobboteck/JoyStick
    * License       : The MIT License (MIT)
    */
    let StickStatus = { xPosition: 0, yPosition: 0, x: 0, y: 0, cardinalDirection: "C" }; var JoyStick = function (t, e, i) { var o = void 0 === (e = e || {}).title ? "joystick" : e.title, n = void 0 === e.width ? 0 : e.width, a = void 0 === e.height ? 0 : e.height, r = void 0 === e.internalFillColor ? "#00AA00" : e.internalFillColor, s = void 0 === e.internalLineWidth ? 2 : e.internalLineWidth, c = void 0 === e.internalStrokeColor ? "#003300" : e.internalStrokeColor, $ = void 0 === e.externalLineWidth ? 2 : e.externalLineWidth, d = void 0 === e.externalStrokeColor ? "#008000" : e.externalStrokeColor, u = void 0 === e.autoReturnToCenter || e.autoReturnToCenter; i = i || function (t) { }; var h = document.getElementById(t); h.style.touchAction = "none"; var S = document.createElement("canvas"); S.id = o, 0 === n && (n = h.clientWidth), 0 === a && (a = h.clientHeight), S.width = n, S.height = a, h.appendChild(S); var f = S.getContext("2d"), l = 0, k = 2 * Math.PI, _ = (S.width - (S.width / 2 + 10)) / 2, g = _ + 5, x = _ + .6 * _, v = S.width / 2, P = S.height / 2, C = S.width / 10, p = -1 * C, y = S.height / 10, w = -1 * y, L = v, F = P; function m() { f.beginPath(), f.arc(v, P, x, 0, k, !1), f.lineWidth = $, f.strokeStyle = d, f.stroke() } function E() { f.beginPath(), L < _ && (L = g), L + _ > S.width && (L = S.width - g), F < _ && (F = g), F + _ > S.height && (F = S.height - g), f.arc(L, F, _, 0, k, !1); var t = f.createRadialGradient(v, P, 5, v, P, 200); t.addColorStop(0, r), t.addColorStop(1, c), f.fillStyle = t, f.fill(), f.lineWidth = s, f.strokeStyle = c, f.stroke() } function W(t) { l = 1 } function T(t) { l = 1 } function D() { let t = "", e = L - v, i = F - P; return i >= w && i <= y && (t = "C"), i < w && (t = "N"), i > y && (t = "S"), e < p && ("C" === t ? t = "W" : t += "W"), e > C && ("C" === t ? t = "E" : t += "E"), t } "ontouchstart" in document.documentElement ? (S.addEventListener("touchstart", W, !1), document.addEventListener("touchmove", function t(e) { 1 === l && e.targetTouches[0].target === S && (L = e.targetTouches[0].pageX, F = e.targetTouches[0].pageY, "BODY" === S.offsetParent.tagName.toUpperCase() ? (L -= S.offsetLeft, F -= S.offsetTop) : (L -= S.offsetParent.offsetLeft, F -= S.offsetParent.offsetTop), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus)) }, !1), document.addEventListener("touchend", function t(e) { l = 0, u && (L = v, F = P), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus) }, !1)) : (S.addEventListener("mousedown", T, !1), document.addEventListener("mousemove", function t(e) { 1 === l && (L = e.pageX, F = e.pageY, "BODY" === S.offsetParent.tagName.toUpperCase() ? (L -= S.offsetLeft, F -= S.offsetTop) : (L -= S.offsetParent.offsetLeft, F -= S.offsetParent.offsetTop), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus)) }, !1), document.addEventListener("mouseup", function t(e) { l = 0, u && (L = v, F = P), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus) }, !1)), m(), E(), this.GetWidth = function () { return S.width }, this.GetHeight = function () { return S.height }, this.GetPosX = function () { return L }, this.GetPosY = function () { return F }, this.GetX = function () { return (100 * ((L - v) / g)).toFixed() }, this.GetY = function () { return (-(100 * ((F - P) / g) * 1)).toFixed() }, this.GetDir = function () { return D() } };
</script>
</head>

<body>
<section class="debug"></section>

<section class="container">
    <div class="joystick">
        <div id="joystick-canvas"></div>
    </div>
    <div class="video">
       <img id="stream" src="">
    </div>
    <div class="buttons">
        <button type="button" id="button-a">LED</button>
        <button type="button" id="button-b">Lights</button>
    </div>
</section>

<script>
class SocketClient {
    constructor(port) {
        this.port = port;
        this.connection = new WebSocket(`ws://${window.location.hostname}:${port}`);
        this.connection.onopen = () => this.onOpen();
        this.connection.onerror = (error) => this.onError(error);
    }

    send(params) {
        const jsonString = JSON.stringify(params);
        console.log(jsonString);
        document.querySelector('.debug').innerHTML = jsonString;
        this.connection.send(jsonString);
    }

    setOnMessage(handler) {
        this.connection.onmessage = (e) => handler(e);
    }

    onOpen() {
        console.log('WebSocket Connection at ' + window.location.hostname);
    }

    onError(error) {
        console.log('WebSocket Error ' + error);
        alert('WebSocket Error #' + error);
    }
}

class MessageHandler {
    constructor(socketClient) {
        this.socketClient = socketClient
        this.socketClient.setOnMessage(this.handle);
    }

    handle(e) {
        console.log('Message received: ' + e.data);
    }
}

class ToggleButton {
    constructor(selector,
                callback,
                initivalValue = false) {
        this.element = document.querySelector(selector);
        this.callback = callback;
        this.setValue(initivalValue);
        this.setEvent();
    }

    setEvent() {
        this.element.addEventListener('click', () => {
            this.toggle();
            this.callback(this.value);
        });
    }

    toggle() {
        this.setValue(!this.value);
    }

    setValue(value) {
        this.value = value;
        this.element.classList.toggle('pressed', value);
    }
}

class PushButton {
    constructor(selector,
                callback,
                initivalValue = false) {
        this.element = document.querySelector(selector);
        this.callback = callback;
        this.setValue(initivalValue);
        this.timer = null;
        this.setEvent();
    }

    setEvent() {
        this.element.addEventListener('pointerdown', () => {
            this.setValue(true);
            this.callback(this.value);
            this.timer = window.setInterval(() => this.callback(this.value), 290);
        });

        this.element.addEventListener('pointerup', () => {
            window.clearInterval(this.timer);
            this.setValue(false);
            this.callback(this.value);
        });
    }

    setValue(value) {
        this.value = value;
        this.element.classList.toggle('pressed', value);
    }
}

class MessageSender {
    constructor(socketClient, 
                joystick, 
                buttonASel, 
                buttonBSel) {
        this.socketClient = socketClient;
        this.joystick = joystick;
        this.buttonA = new ToggleButton(buttonASel, (value) => this.sendButtonA(value));
        this.buttonB = new ToggleButton(buttonBSel, (value) => this.sendButtonB(value));

        window.setInterval(() => this.sendJoystick(), 50)
    }

    sendButtonA(value) {
        const params = { 'button-a': value };

        this.socketClient.send(params);
    }
    
    sendButtonB(value) {
        const params = { 'button-b': value };

        this.socketClient.send(params);
    }

    sendJoystick() {
        const xAxis = this.joystick.GetX();
        const yAxis = this.joystick.GetY();
        const direction = this.joystick.GetDir();
        const speed = Math.abs(xAxis) > Math.abs(yAxis)
            ? Math.abs(xAxis)
            : Math.abs(yAxis);
        const params = { direction, speed };

        this.socketClient.send(params);
    }
}

class VideoStream {
  constructor(selector, port) {
    this.element = document.querySelector(selector);
    this.port = port;
    this.init();
  }

  getUrl() {
    return window.location.href.slice(0, -1) + ":" + this.port + "/stream";
  }

  init() {
    this.element.src = this.getUrl();
  }
}

window.addEventListener('load', () => {
    const socketClient = new SocketClient(82);
    const messageHandler = new MessageHandler(socketClient);
    const joystick = new JoyStick('joystick-canvas', {
        'internalFillColor': '#444',
        'internalStrokeColor': '#222',
        'externalStrokeColor': '#222'
    });
    const messageSender = new MessageSender(
        socketClient,
        joystick,
        '#button-a',
        '#button-b'
    );
    const videoStream = new VideoStream(
      '#stream',
      81
    );
});
</script>

</body>
</html>
)rawliteral";

void StreamServer::init() {

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //disable brownout detector

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 15;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 15;
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}


esp_err_t StreamServer::index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

esp_err_t StreamServer::stream_handler(httpd_req_t *req) {
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t *_jpg_buf = NULL;
  char *part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);

  if (res != ESP_OK) {
    return res;
  }

  while (true) {
    fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if (fb->width > 400) {
        if (fb->format != PIXFORMAT_JPEG) {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted) {
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }

    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }

    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }

    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }

    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }

    if (res != ESP_OK) {
      break;
    }
  }
  return res;
}


void StreamServer::startStream() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = StreamServer::index_handler,
    .user_ctx = NULL
  };

  httpd_uri_t stream_uri = {
    .uri = "/stream",
    .method = HTTP_GET,
    .handler = StreamServer::stream_handler,
    .user_ctx = NULL
  };

  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;

  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

