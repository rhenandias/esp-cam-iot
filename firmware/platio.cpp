#include "platio.h"

PlatIO::PlatIO() {}

// Inicializa a biblioteca
void PlatIO::begin(Client &client, const char serverName[], const int serverPort, bool mode)
{
  this->client = &client;               // Wifi Cient, inicializado na sketch
  this->mode = mode;                    // Modo de operação (true para local, false para online)
  this->serverPort = serverPort;        // Porta para conexão ao servidor

  strcpy(this->serverName, serverName); // Nome do servidor para conexão

  // Monta o inínio da url de API separado para os modos LOCAL e ONLINE
  if (this->mode)
  {
    // Local Mode
    // http://192.168.x.xx:xxxx
    strcat(this->targetUrl, "http://");
    strcat(this->targetUrl, this->serverName);
    strcat(this->targetUrl, ":");
    char buf[5];
    itoa(this->serverPort, buf, 10);
    strcat(this->targetUrl, buf);
  }
  else
  {
    // Online Mode
    // https://plat-io.herokuapp.com
    strcat(this->targetUrl, "https://");
    strcat(this->targetUrl, this->serverName);
  }
}

// Realiza leitura do valor de uma flag
String PlatIO::readFlag(const char path[])
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/readflag?path=");
  strcat(url, path);

  char *url_ptr = url;

  HTTPClient http;
  http.begin(url_ptr);

  String response;
  response.reserve(32);

  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String payload = http.getString();
    response = getJSONValueByKey(payload.c_str(), "value");
  }
  else
  {
    Serial.println("Error on HTTP request");
    response = String(httpCode);
  }
  http.end();

  return response;
}

// Sobrescreve o valor de uma flag com parâmetro "int"
int PlatIO::setFlag(const char path[], const int value)
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/setflag?path=");
  strcat(url, path);
  strcat(url, "&value=");

  char buf[8] = {0};
  itoa(value, buf, 10);
  strcat(url, buf);

  char *url_ptr = url;

  return this->postRaw(url_ptr);
}

// Sobrescreve o valor de uma flag com parâmetro "float"
int PlatIO::setFlag(const char path[], const float value)
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/setflag?path=");
  strcat(url, path);
  strcat(url, "&value=");

  char buf[16] = {0};
  sprintf(buf, "%.2f", value);
  strcat(url, buf);

  char *url_ptr = url;

  return this->postRaw(url_ptr);
}

// Sobrescreve o valor de uma flag com parâmetro "char[]"
int PlatIO::setFlag(const char path[], const char value[])
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/setflag?path=");
  strcat(url, path);
  strcat(url, "&value=");
  strcat(url, value);

  char *url_ptr = url;

  return this->postRaw(url_ptr);
}

// Realiza leitura do último valor inserido em um field
String PlatIO::readField(const char path[])
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/readfield?path=");
  strcat(url, path);

  char *url_ptr = url;

  HTTPClient http;
  http.begin(url_ptr);

  String response;
  response.reserve(32);

  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String payload = http.getString();
    response = getJSONValueByKey(payload.c_str(), "value");
  }
  else
  {
    Serial.println("Error on HTTP request");
    response = String(httpCode);
  }
  http.end();

  return response;
}

// Insere um registro em um campo com parâmetro "int"
int PlatIO::pushField(const char path[], const int value)
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/pushfield?path=");
  strcat(url, path);
  strcat(url, "&value=");

  char buf[8] = {0};
  itoa(value, buf, 10);
  strcat(url, buf);

  char *url_ptr = url;

  return this->postRaw(url_ptr);
}

// Insere um registro em um campo com parâmetro "float"
int PlatIO::pushField(const char path[], const float value)
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/pushfield?path=");
  strcat(url, path);
  strcat(url, "&value=");

  char buf[16] = {0};
  sprintf(buf, "%.2f", value);
  strcat(url, buf);

  char *url_ptr = url;

  return this->postRaw(url_ptr);
}

// Insere um registro em um campo com parâmetro "char[]"
int PlatIO::pushField(const char path[], const char value[])
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/pushfield?path=");
  strcat(url, path);
  strcat(url, "&value=");
  strcat(url, value);

  char *url_ptr = url;

  return this->postRaw(url_ptr);
}

// Realiza upload de um arquivo (por enquanto configurado para foto apenas)
int PlatIO::upload(const char path[], uint8_t *buf, size_t len)
{
  char url[80] = {0};

  strcat(url, this->targetUrl);
  strcat(url, "/api/pushphoto?path=");
  strcat(url, path);

  Serial.println(url);

  Serial.print("Connecting to server: ");
  Serial.println(this->serverName);
  Serial.println(this->serverPort);

  if (this->client->connect(this->serverName, this->serverPort))
  {
    Serial.println("Connection successful!");

    String head = "--EspCamIOT\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--EspCamIOT--\r\n";

    uint16_t imageLen = len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;

    this->client->println("POST " + String(url) + " HTTP/1.1");
    this->client->println("Host: " + String(this->serverName));
    this->client->println("Content-Length: " + String(totalLen));
    this->client->println("Content-Type: multipart/form-data; boundary=EspCamIOT");
    this->client->println();
    this->client->print(head);

    uint8_t *fbBuf = buf;
    size_t fbLen = len;
    for (size_t n = 0; n < fbLen; n = n + 1024)
    {
      if (n + 1024 < fbLen)
      {
        this->client->write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen % 1024 > 0)
      {
        size_t remainder = fbLen % 1024;
        this->client->write(fbBuf, remainder);
      }
    }
    this->client->print(tail);
  }
  else
  {
    Serial.println("Connection Failure!");
  }
  return 0;
}

// Método POST genérico em uma url
int PlatIO::postRaw(const char *url)
{
  HTTPClient http;
  http.begin(url);

  const int httpCode = http.POST("");
  if (httpCode > 0)
  {
    // Por enquanto não queremos fazer nada com o payload de resposta
    // String payload = http.getString();
  }
  else
  {
    Serial.println("Error on HTTP request");
  }
  http.end();

  return httpCode;
}

// Adquire o valor de uma chave em um objeto JSON representado em uma string
String PlatIO::getJSONValueByKey(String textToSearch, String key)
{
  if (textToSearch.length() == 0)
  {
    return String("");
  }

  String searchPhrase = String("\"");
  searchPhrase.concat(key);
  searchPhrase.concat("\":\"");

  int fromPosition = textToSearch.indexOf(searchPhrase, 0);

  if (fromPosition == -1)
  {
    // return because there is no status or it's null
    return String("");
  }

  fromPosition = fromPosition + searchPhrase.length();

  int toPosition = textToSearch.indexOf("\"", fromPosition);

  if (toPosition == -1)
  {
    // return because there is no end quote
    return String("");
  }

  textToSearch.remove(toPosition);

  return textToSearch.substring(fromPosition);
}
