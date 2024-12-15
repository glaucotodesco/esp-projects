#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <NTPClient.h>
#include <time.h>

#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define BUZZER_PIN 4
#define DHTPIN 5         // GPIO conectado ao DATA do DHT
#define DHTTYPE DHT11    // Substitua por DHT22 se for o caso
#define LED 19

void setupServer();
void setupSensor();
void setupWIFI();
void setupLed();
void setupBuzzer();
void setupOled();

void handleRequest();
void requestLed();
void displayData();

void playBuzzer();
void taskServer(void *parameter);
void taskDisplay(void *parameter);
void taskBuzzer(void *parameter);

String getFormatedDate( struct tm * timeInfo);
String getFormatedTime( struct tm * timeInfo);
struct tm *getDateTime();

// Configurações Wi-Fi
const char *ssid = "gt";
const char *password = "todesco1";
int cont = 0;

// Configuração do servidor NTP e fuso horário
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600; // Ajuste para seu fuso horário (GMT-3)
const int daylightOffset_sec = 0;     // Ajuste para horário de verão, se necessário

// Configuração do servidor web
WebServer server(80);

// Sersor
DHT dht(DHTPIN, DHTTYPE);

// Oled
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// time
WiFiUDP udp;
NTPClient timeClient(udp, ntpServer, gmtOffset_sec, 60000); // Atualiza a cada 60 segundos


String formatedTime;
String formatedDate;
float temperature;
float humidity;
bool request = false;

void setup()
{
  Serial.begin(9600);
  setupWIFI();
  setupSensor();
  setupLed();
  setupServer();
  setupOled();
  setupBuzzer();

  xTaskCreatePinnedToCore(taskServer, "WebServerTask", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskDisplay, "DisplayTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskBuzzer, "PlayBuzzerTask", 2048, NULL, 1, NULL, 1);
}

void loop()
{
  struct tm * timeInfo= getDateTime();
   
  // get date and time;
  formatedDate = getFormatedDate(timeInfo);
  formatedTime = getFormatedTime(timeInfo);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  delay(1000);
}

void taskBuzzer(void *parameter)
{
  for (;;)
  {
    if(request){
      playBuzzer();
      request = false;
    }
    else{
      delay(1000);
    }
    
  }
}


void taskServer(void *parameter)
{
  for (;;)
  {
    server.handleClient(); // Responde às requisições HTTP
  }
}

void playBuzzer()
{
  Serial.println("Play Buzzer");
  // Liga o buzzer
  digitalWrite(BUZZER_PIN, HIGH); 
  delay(1000); // Mantém ligado por 1 segundo

  // Desliga o buzzer
  digitalWrite(BUZZER_PIN, LOW); 
  delay(1000); // Mantém desligado por 1 segundo
}

void taskDisplay(void *parameter)
{
  for (;;)
  {
    displayData();
    delay(1000);
  }
}

struct tm *getDateTime(){
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    return gmtime(&epochTime);
}

void setupServer()
{
  server.on("/sensors", handleRequest);
  server.begin();
}

void setupBuzzer(){
  pinMode(BUZZER_PIN, OUTPUT); // Configura o pino como saída
}

void setupOled()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
}

String getFormatedDate( struct tm * timeInfo){
  return String(timeInfo->tm_mday) + "/" + String(timeInfo->tm_mon + 1) + "/" + String(timeInfo->tm_year + 1900);
}

String getFormatedTime( struct tm * timeInfo){
  return String(timeInfo->tm_hour) + ":" + String(timeInfo->tm_min) + ":" + String(timeInfo->tm_sec);
}

void displayData()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print(formatedDate);
  display.setCursor(0, 9);
  display.print(formatedTime);

  display.setCursor(0, 30);
  display.print("Temperatura: ");
  display.print(temperature);
  display.setCursor(0, 50);
  display.print("Umidade: ");
  display.print(humidity);
  display.display();
  delay(100);
}

void handleRequest()
{
  Serial.println("Handle a client...");
  
  request = true;

  // Verifica se as leituras são válidas
  if (isnan(temperature) || isnan(humidity))
  {
    server.send(500, "application/json", "{\"erro\": \"Falha na leitura do sensor\"}");
  }
  else
  {
    // Cria um objeto JSON
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["date"] = formatedDate;
    doc["time"] = formatedTime;

    // Serializa o JSON e envia como resposta
    String jsonString;
    serializeJson(doc, jsonString);
    server.send(200, "application/json", jsonString);

    requestLed();
  }
}

void setupLed()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void setupSensor()
{
  dht.begin();
}

void setupWIFI()
{
  // Conectar ao Wi-Fi
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void requestLed()
{
  digitalWrite(LED, LOW); // Liga o LED
  delay(1000);             // Espera 1 segundo
  digitalWrite(LED, HIGH);  // Desliga o LED
  delay(1000);             // Espera 1 segundo
}
