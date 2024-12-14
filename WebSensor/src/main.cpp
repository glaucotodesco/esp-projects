#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

void setupServer();
void setupSensor();
void setupWIFI();
void setupLed();
void handleRequest();
void requestLed();


// Configurações Wi-Fi
const char *ssid = "gt";
const char *password = "todesco1";

// Configuração do sensor DHT
#define DHTPIN 5      // GPIO conectado ao DATA do DHT
#define DHTTYPE DHT11 // Substitua por DHT22 se for o caso


#define LED 19

// Configuração do servidor web
WebServer server(80);

//Sersor
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);

  setupWIFI();
  setupSensor();
  setupLed();
  setupServer();
  
}

void loop()
{
  server.handleClient();
}

void setupServer()
{
  server.on("/sensors", handleRequest);
  server.begin();
}

void handleRequest()
{
  Serial.println("Handle a client...");

  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  
  // Verifica se as leituras são válidas
  if (isnan(temperatura) || isnan(umidade))
  {
    server.send(500, "application/json", "{\"erro\": \"Falha na leitura do sensor\"}");
  }
  else
  {
    // Cria um objeto JSON
    StaticJsonDocument<200> doc;
    doc["temperatura"] = temperatura;
    doc["umidade"] = umidade;

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
  digitalWrite(LED, HIGH); // Liga o LED
  delay(1000);             // Espera 1 segundo
  digitalWrite(LED, LOW);  // Desliga o LED
  delay(1000);             // Espera 1 segundo
}

