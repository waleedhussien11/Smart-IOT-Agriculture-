#include <DHT.h>
#include <Wire.h> // Required for I2C communication
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial nodeMCUSerial(2, 3);
#define dht_pin 8
#define dhtType DHT11
#define AO_PIN A0
#define AOUT_PIN A1 // Arduino pin that connects to AOUT pin of moisture sensor
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 (from DIYables LCD), 16 column and 2 rows

DHT dht(dht_pin, dhtType);

void setup() {

  Serial.begin(9600);
  nodeMCUSerial.begin(9600);
  dht.begin();
  lcd.init();      // initialize the lcd
  lcd.backlight();

}
void loop () {
  float t = dht.readTemperature();           // Read temperature in Celsius
  float h = dht.readHumidity();
  int gasValue = analogRead(AO_PIN);
  int value = analogRead(AOUT_PIN);
  // read the analog value from sensor
  if (isnan(t) || isnan(h) ) {
    Serial.println("NO data found " ) ;
  }

  Serial.print(t);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.print(gasValue);
  Serial.print(",");
  Serial.print(value);
  Serial11111.println();
  delay(5000);
  lcd.setCursor(0, 0);  // start to print at the first row
  lcd.print("Temp: ");
  lcd.print(t);     // print the temperature
  lcd.print((char)223); // print ° character
  lcd.print("C");
  lcd.setCursor(0, 1);  // start to print at the second row
  lcd.print("Humi: ");
  lcd.print(h);      // print the humidity
  lcd.print("%");
  delay(5000); 
}
