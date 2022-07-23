#include <Arduino.h>
#include <EEPROM.h>

#define LEDPin LED_BUILTIN
#define CTRLPin 3
#define DHTPin 2
#define EEPROM_ADR 0

unsigned int counter = 0;
unsigned int i=0;


void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin,OUTPUT);
  pinMode(CTRLPin, OUTPUT);
  Serial.begin(9600);
  counter = EEPROM.read(EEPROM_ADR);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(LEDPin,HIGH);
  digitalWrite(CTRLPin,HIGH);*/
  for(i=0; i<1000; i++)
  {
    delay(1000);
    Serial.print("counter = ");
    Serial.println(counter);
    EEPROM.write(EEPROM_ADR,counter);
    counter = counter+1;
  }
}