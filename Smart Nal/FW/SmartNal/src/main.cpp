//#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
// #include "LibPrintf.h"
//#include <DHT_U.h>

#define DHTPIN 2 // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT22       // DHT 22  (AM2302), AM2321
#define DHTTYPE DHT11 // DHT 11

#define LEDPin LED_BUILTIN  // Arduino nano buildin LED PIN
#define CTRLPin 3           // Assign D3 to Control
#define EEPROM_ADR 10        // EEPROM ADDRESS 0
#define maxReset 1          // Maximum number of reset

DHT dht(DHTPIN, DHTTYPE);   // Initialize DHT11
volatile uint8_t lightStatus = 0;
volatile unsigned char serialMessage = 0;
volatile uint8_t resetCounter = 0;
float humidityResult = 0;
float tempResult = 0;

void setup()
{
  pinMode(LEDPin, OUTPUT);  // Configure LED Pin as an OUTPUT
  pinMode(CTRLPin, OUTPUT); // Configure controll pin as an OUTPUT
  Serial.begin(9600);        // Start Serial communication
  dht.begin();               // Start DHT11
  digitalWrite(LEDPin, LOW);
  
  resetCounter = EEPROM.read(EEPROM_ADR); // Assign EEPROM value stored in ADDRESS 0 to the variable resetCounter

  resetCounter++;                         // Increament resetCounter
  if (resetCounter > maxReset)
  {
    lightStatus = 1;
    resetCounter = 0;               // set the variable resetCounter to 0
    // digitalWrite(LEDPin, HIGH);
    digitalWrite(CTRLPin, HIGH);
  }
  else
  {
    lightStatus = 0;
    // digitalWrite(LEDPin, LOW);
    digitalWrite(CTRLPin, LOW);
  }
  EEPROM.write(EEPROM_ADR, resetCounter); // Store the new resetCounter to EEPROM
}

void loop()
{
  noInterrupts();
  // critical, time-sensitive code here
  if (lightStatus == 0)
  {
    // digitalWrite(LEDPin, LOW);
    digitalWrite(CTRLPin, LOW);
  }
  else if (lightStatus == 1)
  {
    // digitalWrite(LEDPin, HIGH);
    digitalWrite(CTRLPin, HIGH);
    resetCounter = 0; // set the variable resetCounter to 0
  }
  
  interrupts();
  delay(100);
  humidityResult = dht.readHumidity();    // Read humidity as procent (the default)
  tempResult = dht.readTemperature(); // Read temperature as Celsius (the default)
  EEPROM.write(EEPROM_ADR, resetCounter);
}

// Interrupt on serial event
void serialEvent()
{
  while (Serial.available())
  {
    serialMessage = Serial.read();
  }
  Serial.println();
  Serial.print("Temp=");
  Serial.println(tempResult);
  Serial.print("Hum =");
  Serial.println(humidityResult);

  if (serialMessage == '1')
  {
    lightStatus = 1;
    
  }
  else if (serialMessage == '0')
  {
    lightStatus = 0;
  }
}