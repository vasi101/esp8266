#define BLYNK_TEMPLATE_ID "TMPL62IsoMLHl"
#define BLYNK_TEMPLATE_NAME "nodmcu"
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define GPIO pins connected to relays
#define RelayPin1 5  // D1
#define RelayPin2 4  // D2
#define RelayPin3 0  // D3
#define RelayPin4 2  // D4

// Define virtual pins for controlling relays
#define VPIN_RELAY_1 V1
#define VPIN_RELAY_2 V2
#define VPIN_RELAY_3 V3
#define VPIN_RELAY_4 V4

// WiFi credentials
#define WIFI_SSID "Home WiFi"     // Replace with your WiFi SSID
#define WIFI_PASS "HOMEWIFI98297193" // Replace with your WiFi password

// Function to set relay state
void controlRelay(int relayPin, int state) {
  digitalWrite(relayPin, state == 1 ? HIGH : LOW);
}

BLYNK_WRITE(VPIN_RELAY_1) {
  int relayState = param.asInt(); // Get the relay state (0 or 1)
  controlRelay(RelayPin1, relayState);
}

BLYNK_WRITE(VPIN_RELAY_2) {
  int relayState = param.asInt(); // Get the relay state (0 or 1)
  controlRelay(RelayPin2, relayState);
}

BLYNK_WRITE(VPIN_RELAY_3) {
  int relayState = param.asInt(); // Get the relay state (0 or 1)
  controlRelay(RelayPin3, relayState);
}

BLYNK_WRITE(VPIN_RELAY_4) {
  int relayState = param.asInt(); // Get the relay state (0 or 1)
  controlRelay(RelayPin4, relayState);
}

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Set relay pins as OUTPUT
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  // Initialize relays in OFF state
  digitalWrite(RelayPin1, LOW);
  digitalWrite(RelayPin2, LOW);
  digitalWrite(RelayPin3, LOW);
  digitalWrite(RelayPin4, LOW);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Connect to Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);
  while (!Blynk.connected()) {
    Blynk.connect();
  }
}

void loop() {
  Blynk.run(); // Run Blynk
}
