#ifndef PLAT_IO
#define PLAT_IO

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class PlatIO {
  public:
    PlatIO();

    // Inicialização
    void begin(Client &client, const char serverName[], const int serverPort, bool mode);

    // Sobrescreve o valor de uma flag
    int setFlag(const char path[], const int value);
    int setFlag(const char path[], const float value);
    int setFlag(const char path[], const char value[]);

    // Realiza leitura do valor de uma flag
    String readFlag(const char path[]);

    // Insere um registro em um campo
    int pushField(const char path[], const int value);
    int pushField(const char path[], const float value);
    int pushField(const char path[], const char value[]);

    // Realiza leitura do último valor inserido em um field
    String readField(const char path[]);

    // Realiza upload
    int upload(const char path[], uint8_t *buf, size_t len);

  private:
    char serverName[70];
    char targetUrl[70];
    int serverPort = 0;
    bool mode;

    Client *client = NULL;

    int postRaw(const char *url);

    String getJSONValueByKey(String textToSearch, String key);
};

#endif