#include <WiFi.h> // Para ESP32
// #include <ESP8266WiFi.h> // Para ESP8266, descomente esta linha

// Configurações Wi-Fi
const char* ssid = "gt";              // Substitua pelo nome da sua rede Wi-Fi
const char* password = "todesco1";    // Substitua pela senha do Wi-Fi


void setup() {

  pinMode(4, OUTPUT); // GPIO 2 é geralmente ligado ao LED embutido

  Serial.begin(9600); // Inicializa o monitor serial
  Serial.println("Conectando ao Wi-Fi...");

  WiFi.begin(ssid, password); // Inicia a conexão Wi-Fi

  // Aguarda até conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Conectado!
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    digitalWrite(4, HIGH); // Liga o LED
    delay(1000);           // Espera 1 segundo
    digitalWrite(4, LOW);  // Desliga o LED
    delay(1000);   
}
