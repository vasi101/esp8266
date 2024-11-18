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

// Variables to store switch states
int switchState[] = {0, 0, 0, 0};
int lastSwitchState[] = {0, 0, 0, 0};
int relayState[] = {0, 0, 0, 0};

// Blynk virtual pins for each relay
const int VPIN_START = V1;

// Initialize Blynk
BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  
  // Initialize relay pins as outputs
  for(int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW);  // Start with relays OFF
  }
  
  // Initialize switch pins as inputs with pullup
  for(int i = 0; i < 4; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
  }
  
  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Setup periodic sync with Blynk server
  timer.setInterval(1000L, checkPhysicalSwitches);
}

// Sync states with Blynk app
BLYNK_CONNECTED() {
  // Sync all states when reconnected
  for(int i = 0; i < 4; i++) {
    Blynk.virtualWrite(VPIN_START + i, relayState[i]);
  }
}

// Blynk handlers for virtual pins
BLYNK_WRITE(V1) { relayControl(0, param.asInt()); }
BLYNK_WRITE(V2) { relayControl(1, param.asInt()); }
BLYNK_WRITE(V3) { relayControl(2, param.asInt()); }
BLYNK_WRITE(V4) { relayControl(3, param.asInt()); }

// Function to control relay and update Blynk
void relayControl(int relay, int state) {
  relayState[relay] = state;
  digitalWrite(RELAY_PINS[relay], state);
  Blynk.virtualWrite(VPIN_START + relay, state);
}

// Function to handle physical switch changes
void handlePhysicalSwitch(int switchIndex) {
  switchState[switchIndex] = digitalRead(SWITCH_PINS[switchIndex]);
  
  if(switchState[switchIndex] != lastSwitchState[switchIndex]) {
    delay(50); // Simple debounce
    
    // Toggle relay state on button press
    if(switchState[switchIndex] == LOW) {
      relayState[switchIndex] = !relayState[switchIndex];
      relayControl(switchIndex, relayState[switchIndex]);
    }
    
    lastSwitchState[switchIndex] = switchState[switchIndex];
  }
}

// Function to check all physical switches
void checkPhysicalSwitches() {
  for(int i = 0; i < 4; i++) {
    handlePhysicalSwitch(i);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
