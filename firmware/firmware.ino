#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "platio.h"

//Configuração da rede WiFi
const char *ssid = "VIVO-70D4";
const char *password = "aNU3497933";

WiFiClient client;
PlatIO platio;

String response;

// #define LOCAL true

// Configuração do modelo de câmera (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_hex_color 32
#define RESET_GPIO_hex_color -1
#define XCLK_GPIO_hex_color 0
#define SIOD_GPIO_hex_color 26
#define SIOC_GPIO_hex_color 27
#define Y9_GPIO_hex_color 35
#define Y8_GPIO_hex_color 34
#define Y7_GPIO_hex_color 39
#define Y6_GPIO_hex_color 36
#define Y5_GPIO_hex_color 21
#define Y4_GPIO_hex_color 19
#define Y3_GPIO_hex_color 18
#define Y2_GPIO_hex_color 5
#define VSYNC_GPIO_hex_color 25
#define HREF_GPIO_hex_color 23
#define PCLK_GPIO_hex_color 22

void setup()
{
  Serial.begin(115200);

  // Configuração da câmetra
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_hex_color;
  config.pin_d1 = Y3_GPIO_hex_color;
  config.pin_d2 = Y4_GPIO_hex_color;
  config.pin_d3 = Y5_GPIO_hex_color;
  config.pin_d4 = Y6_GPIO_hex_color;
  config.pin_d5 = Y7_GPIO_hex_color;
  config.pin_d6 = Y8_GPIO_hex_color;
  config.pin_d7 = Y9_GPIO_hex_color;
  config.pin_xclk = XCLK_GPIO_hex_color;
  config.pin_pclk = PCLK_GPIO_hex_color;
  config.pin_vsync = VSYNC_GPIO_hex_color;
  config.pin_href = HREF_GPIO_hex_color;
  config.pin_sscb_sda = SIOD_GPIO_hex_color;
  config.pin_sscb_scl = SIOC_GPIO_hex_color;
  config.pin_pwdn = PWDN_GPIO_hex_color;
  config.pin_reset = RESET_GPIO_hex_color;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  // Conexão WiFi
  Serial.println(F("Conectando wifi"));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println(F("\nConectado"));

  // Configura biblioteca
  if (false)
    platio.begin(client, "192.168.15.5", 4000, true);
  else
    platio.begin(client, "esp-cam-iot.herokuapp.com", 80, false);

  // Iniciação da câmera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Aloca espaço para a string que receberá respostas da biblioteca
  response.reserve(8);
}

void sendPhoto()
{
  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();

  if (!fb)
  {
    // Serial.println("Camera capture failed");
    Serial.println(F("Falha na captura de imagem"));
  }
  else
  {
    Serial.println(F("Sucesso na captura de imagem"));
    platio.upload("camera/live", fb->buf, fb->len);
  }
  Serial.println(F("Ciclo de captura completo"));

  if (fb)
  {
    esp_camera_fb_return(fb);
  }
}

void loop()
{
  response = platio.readFlag("updates");
  if(response == "1")
  {
    Serial.println("Tirar foto agora");
    sendPhoto();
    Serial.println("Resetar flag agora");
    platio.setFlag("updates", 0);
  } else {
    Serial.println("Nenhuma atualização ...");
  }

  delay(1000);
}
