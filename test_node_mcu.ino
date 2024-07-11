#include <ESP8266WiFi.h>                        // Include ESP8266WiFi library for WiFi features
#include <FirebaseESP8266.h>
#include <SoftwareSerial.h>
#include <Servo.h> // include servo library to use its related functions
#define Servo_PWM D6 // A descriptive name for D6 pin of Arduino to provide PWM signal
Servo MG995_Servo;  // Define an instance of of Servo with the name of "MG995_Servo"
#define fan_pin D5
const char* FIREBASE_HOST = "iot-smart-agriculture-91d92-default-rtdb.firebaseio.com";
const char* FIREBASE_AUTH = "SYIK23ziZ5qVHNqo3bzzEMlcVBgXG54dNxWmMFW0";
const char* WIFI_SSID = "Etisalat-2XxT";               // wifi name
const char* WIFI_PASSWORD = "FEPZDNhh";
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String currentDoorState = "";  // To track the current state of the door
int IN1 = D7;
int IN2 = D8;
String door;
String pump;
String pump_state;
String fan_state;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  pinMode(fan_pin, OUTPUT);
  digitalWrite(fan_pin,LOW);
  MG995_Servo.attach(Servo_PWM);
  MG995_Servo.write(0);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);//print local IP address

}

void loop() {
  float temp;
  float hum;
  float gas;
  float value;
  delay(2000);                                  // Wait 2 seconds between measurements

  if (Firebase.getString(fbdo, "/door")) { // Try to get the door status from Firebase
    String door = fbdo.stringData();
    if (door != currentDoorState) {  // Check if the state has changed
      Serial.println("The state of the door is:");
      Serial.println(door);
      Serial.println();

      if (door == "on") {
        Serial.println("we are in deep shit");
        MG995_Servo.write(180);  // Move the servo to 180 degrees
      }
      else if (door == "off") {
        Serial.println("else we are in deep shit");
        MG995_Servo.write(0);    // Move the servo to 0 degrees
      }

      currentDoorState = door;  // Update the current state
    }

    else {
      Serial.println(fbdo.errorReason()); // Print the error reason
    }
  }
  if (Firebase.getString(fbdo, "/fan")) { // Try to get the door status from Firebase
    String fan = fbdo.stringData();
    if (fan != fan_state) {  // Check if the state has changed
      Serial.println("The state of fan is:");
      Serial.println(fan);
      Serial.println();

      if (fan == "on") {
        Serial.println(" the fan state is on ");
        digitalWrite(fan_pin,HIGH);
       Serial.println("on");
      }
      else if (pump == "off") {
        digitalWrite(fan_pin, LOW);
        Serial.println("off");
        // Move the servo to 0 degrees
      }
      fan = fan_state;  // Update the current state
    }
  if (Firebase.getString(fbdo, "/pump")) { // Try to get the door status from Firebase
    String pump = fbdo.stringData();
    if (pump != pump_state) {  // Check if the state has changed
      Serial.println("The state of pump is:");
      Serial.println(pump);
      Serial.println();

      if (pump == "on") {
        Serial.println("we are in deep shit");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        Serial.println("on");
        delay(3000);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        delay(4000); 
        
        
      }
      else if (pump == "off") {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        Serial.println("off");
        delay(1000);
        // Move the servo to 0 degrees
      }

      pump_state = pump;  // Update the current state
    }

    else {
      Serial.println(fbdo.errorReason()); // Print the error reason
    }
  }



  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    Serial.println("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
    int comma1 = data.indexOf(',');
    int comma2 = data.indexOf(',', comma1 + 1);
    int comma3 = data.indexOf(',', comma2 + 1);

    if (comma1 != -1 && comma2 != -1 && comma3 != -1) {
      temp = data.substring(0, comma1).toFloat();
      hum = data.substring(comma1 + 1, comma2).toFloat();
      gas = data.substring(comma2 + 1, comma3).toFloat();
      value = data.substring(comma3 + 1).toFloat();
    }

    if (Firebase.setFloat(fbdo, "/temp", temp)) { // Set temperature value in the Firebase under the "/temp" path
      Serial.print("Temperature: ");              // Print the label "Temperature: "
      Serial.print(temp);                         // Print the temperature value
      Serial.println("°C ");                      // Print the unit "°C"
    }
    else                                          // If Firebase operation fails,
      Serial.println(fbdo.errorReason());

    if (Firebase.setFloat(fbdo, "/hum", hum)) { // Set temperature value in the Firebase under the "/temp" path
      Serial.print("Humadity: ");              // Print the label "Temperature: "
      Serial.print(hum);                         // Print the temperature value
      Serial.println("° ");                      // Print the unit "°C"
    }
    else                                          // If Firebase operation fails,
      Serial.println(fbdo.errorReason());// Print the error reason



    if (Firebase.setFloat(fbdo, "/gas", gas)) { // Set temperature value in the Firebase under the "/temp" path
      Serial.print("Gas is : ");              // Print the label "Temperature: "
      Serial.print(gas);                         // Print the temperature value
      Serial.println(" ");                      // Print the unit "°C"
    }
    else                                          // If Firebase operation fails,
      Serial.println(fbdo.errorReason());

    if (Firebase.setFloat(fbdo, "/value", value)) { // Set temperature value in the Firebase under the "/temp" path
      Serial.print("moisture is : ");              // Print the label "Temperature: "
      Serial.print(value);                         // Print the temperature value
      Serial.println(" ");                      // Print the unit "°C"
    }
    else                                          // If Firebase operation fails,
      Serial.println(fbdo.errorReason());
  }
}

}
