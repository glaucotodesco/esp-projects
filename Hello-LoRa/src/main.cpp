#include <SPI.h>
#include <LoRa.h>

int counter = 0;
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Iniciando Transmissor LoRa...");
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Erro ao iniciar o LoRa!");
    while (1);
  }
  Serial.println("LoRa iniciado com sucesso!");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}