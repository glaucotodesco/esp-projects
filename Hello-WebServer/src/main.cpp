#include <WiFi.h>
#include <WebServer.h> // Para ESP32
// #include <ESP8266WebServer.h> // Para ESP8266

WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello, ESP!");
}

void setup() {
  Serial.begin(9600);

  WiFi.begin("gt", "todesco1");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
