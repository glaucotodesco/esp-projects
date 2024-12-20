#include <SPI.h>
#include <LoRa.h>

int counter = 0;

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

void loop()
{
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