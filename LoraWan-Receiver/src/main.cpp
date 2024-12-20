#include <SPI.h>
#include <LoRa.h>

#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26

void setup() {
  Serial.begin(9600);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  Serial.println("Inicializando LoRa...");
  if (!LoRa.begin(915E6)) {
    Serial.println("Erro: LoRa não inicializou.");
    while (1);
  }
  Serial.println("LoRa inicializado com sucesso!");
}

void loop() {
  // Check for incoming packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Recebido pacote: ");
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.println();
  }
}