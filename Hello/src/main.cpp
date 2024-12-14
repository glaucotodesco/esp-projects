#include <Arduino.h>

void setup() {
    pinMode(4, OUTPUT); // GPIO 2 é geralmente ligado ao LED embutido
}
void loop() {
    digitalWrite(4, HIGH); // Liga o LED
    delay(1000);           // Espera 1 segundo
    digitalWrite(4, LOW);  // Desliga o LED
    delay(1000);           // Espera 1 segundo
}