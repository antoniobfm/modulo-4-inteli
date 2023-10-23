// C++ code
//
int ledPins[] = {13, 12, 8}; // Portas dos LEDs
int contador = 0; // Valor inicial do contador

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT); // Configura cada porta como saída
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[i], bitRead(contador, i)); // Configura o estado de cada LED
  }
  contador++; // Incrementa o contador
  if (contador > 7) contador = 0; // Reseta o contador se ele passar de 7
  delay(1000); // Aguarda 1 segundo
}