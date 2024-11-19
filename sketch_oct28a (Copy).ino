// Define relay pins
#define RELAY1 D5
#define RELAY2 D6
#define RELAY3 D7
#define RELAY4 D8

// Define switch pins
#define SWITCH1 D1
#define SWITCH2 D2
#define SWITCH3 D3
#define SWITCH4 D4

// Variables to store relay states
bool relay1State = LOW;
bool relay2State = LOW;
bool relay3State = LOW;
bool relay4State = LOW;

void setup() {
  // Initialize relay pins as outputs
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Initialize switch pins as inputs with pull-up resistors
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
  pinMode(SWITCH3, INPUT_PULLUP);
  pinMode(SWITCH4, INPUT_PULLUP);

  // Set initial relay states
  digitalWrite(RELAY1, relay1State);
  digitalWrite(RELAY2, relay2State);
  digitalWrite(RELAY3, relay3State);
  digitalWrite(RELAY4, relay4State);
}

void loop() {
  // Check if switches are pressed (LOW because of pull-up resistors)
  if (digitalRead(SWITCH1) == LOW) {
    delay(200); // Debounce delay
    relay1State = !relay1State; // Toggle relay state
    digitalWrite(RELAY1, relay1State);
  }
  if (digitalRead(SWITCH2) == LOW) {
    delay(200); // Debounce delay
    relay2State = !relay2State; // Toggle relay state
    digitalWrite(RELAY2, relay2State);
  }
  if (digitalRead(SWITCH3) == LOW) {
    delay(200); // Debounce delay
    relay3State = !relay3State; // Toggle relay state
    digitalWrite(RELAY3, relay3State);
  }
  if (digitalRead(SWITCH4) == LOW) {
    delay(200); // Debounce delay
    relay4State = !relay4State; // Toggle relay state
    digitalWrite(RELAY4, relay4State);
  }
}
