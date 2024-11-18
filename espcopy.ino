// Blynk authentication details
#define BLYNK_TEMPLATE_ID "IMPL6w817Fd5m"
#define BLYNK_TEMPLATE_NAME "nodemcu"
#define BLYNK_AUTH_TOKEN "f9Q1B40pu9X3zFdtHA2V722GsQTy3fvG"

// Include required libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "WiFi";
char pass[] = "12345678";

// Define pins for relays and switches
const int RELAY_PINS[] = {D1, D2, D3, D4};  // Relay pins
const int SWITCH_PINS[] = {D5, D6, D7, D8}; // Physical switch pins

// Variables to store switch and relay states
int switchState[] = {1, 1, 1, 1}; // HIGH means no press (INPUT_PULLUP)
int lastSwitchState[] = {1, 1, 1, 1};
int relayState[] = {0, 0, 0, 0}; // Relay OFF initially

// Virtual pins for Blynk
const int VPIN_START = V1; // Start virtual pins from V1

// Timer for periodic tasks
BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  // Initialize relay pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW); // Start with relays OFF
  }

  // Initialize switch pins as inputs with pullup resistors
  for (int i = 0; i < 4; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
  }

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Set a timer to check physical switches every 100ms
  timer.setInterval(100L, checkPhysicalSwitches);

  Serial.println("Setup complete!");
}

// Sync relay states when Blynk reconnects
BLYNK_CONNECTED() {
  for (int i = 0; i < 4; i++) {
    Blynk.virtualWrite(VPIN_START + i, relayState[i]); // Sync all relays
  }
}

// Handle Blynk button presses
BLYNK_WRITE(V1) { relayControl(0, param.asInt()); }
BLYNK_WRITE(V2) { relayControl(1, param.asInt()); }
BLYNK_WRITE(V3) { relayControl(2, param.asInt()); }
BLYNK_WRITE(V4) { relayControl(3, param.asInt()); }

// Control relay and sync with Blynk
void relayControl(int relayIndex, int state) {
  relayState[relayIndex] = state;                   // Update relay state
  digitalWrite(RELAY_PINS[relayIndex], state);      // Update relay output
  Blynk.virtualWrite(VPIN_START + relayIndex, state); // Sync Blynk
  Serial.print("Relay "); Serial.print(relayIndex + 1);
  Serial.println(state == HIGH ? " ON" : " OFF");
}

// Check physical switch states
void checkPhysicalSwitches() {
  for (int i = 0; i < 4; i++) {
    int currentState = digitalRead(SWITCH_PINS[i]);

    // If switch state has changed (debounced)
    if (currentState != lastSwitchState[i]) {
      delay(50); // Debounce delay

      if (currentState == LOW) { // Switch pressed
        relayState[i] = !relayState[i]; // Toggle relay state
        relayControl(i, relayState[i]); // Update relay and Blynk
      }

      lastSwitchState[i] = currentState; // Update last state
    }
  }
}

void loop() {
  Blynk.run();  // Handle Blynk connection and communication
  timer.run();  // Check switches periodically
}
