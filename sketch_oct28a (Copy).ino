#define BLYNK_TEMPLATE_ID "TMPL62IsoMLHl"
#define BLYNK_TEMPLATE_NAME "nodmcu"
#define BLYNK_AUTH_TOKEN "rIXhhO8I2liZrCkGe5-qdOK_8n9lsYNh"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials
char ssid[] = "WiFi";       // Replace with your WiFi SSID
char pass[] = "12345678";   // Replace with your WiFi password

// Define relay pins
#define RELAY1 D5
#define RELAY2 D6
#define RELAY3 D7
#define RELAY4 D8

// Setup Blynk virtual pin connections for each relay
BLYNK_WRITE(V1) {
  int state = param.asInt(); // Read value from Virtual Pin V1
  digitalWrite(RELAY1, state);
}

BLYNK_WRITE(V2) {
  int state = param.asInt(); // Read value from Virtual Pin V2
  digitalWrite(RELAY2, state);
}

BLYNK_WRITE(V3) {
  int state = param.asInt(); // Read value from Virtual Pin V3
  digitalWrite(RELAY3, state);
}

BLYNK_WRITE(V4) {
  int state = param.asInt(); // Read value from Virtual Pin V4
  digitalWrite(RELAY4, state);
}

void setup() {
  // Start Serial communication
  Serial.begin(115200);

  // Initialize the relay pins as outputs
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Set default states (relays off)
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Run the Blynk connection
  Blynk.run();
}
