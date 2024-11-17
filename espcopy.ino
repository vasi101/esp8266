#define BLYNK_TEMPLATE_ID "IMPL6w817Fd5m"
#define BLYNK_TEMPLATE_NAME "nodemcu"
#define BLYNK_AUTH_TOKEN "f9Q1B40pu9X3zFdtHA2V722GsQTy3fvG"

#include <BlynkSimpleEsp8266.h>

// Define the GPIO connected with Relays and switches
#define RelayPin1 5  // D1
#define RelayPin2 4  // D2
#define RelayPin3 14 // D5
#define RelayPin4 12 // D6

#define SwitchPin1 10 // SD3
#define SwitchPin2 0  // D3
#define SwitchPin3 13 // D7
#define SwitchPin4 3  // RX

#define wifiLed 16 // D0

#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4

int toggleState_1 = 1;
int toggleState_2 = 1;
int toggleState_3 = 1;
int toggleState_4 = 1;

int wifiFlag = 0;

#define WIFI_SSID "WiFi"         // Enter WiFi Name
#define WIFI_PASS "12345678"     // Enter WiFi Password

BlynkTimer timer;

// Variables for debounce
unsigned long lastDebounceTime1 = 0, lastDebounceTime2 = 0, lastDebounceTime3 = 0, lastDebounceTime4 = 0;
const unsigned long debounceDelay = 50; // 50ms debounce delay

void relayOnOff(int relay) {
    switch (relay) {
        case 1:
            toggleState_1 = !toggleState_1;
            digitalWrite(RelayPin1, toggleState_1 ? HIGH : LOW);
            Serial.println(toggleState_1 ? "Device1 OFF" : "Device1 ON");
            break;
        case 2:
            toggleState_2 = !toggleState_2;
            digitalWrite(RelayPin2, toggleState_2 ? HIGH : LOW);
            Serial.println(toggleState_2 ? "Device2 OFF" : "Device2 ON");
            break;
        case 3:
            toggleState_3 = !toggleState_3;
            digitalWrite(RelayPin3, toggleState_3 ? HIGH : LOW);
            Serial.println(toggleState_3 ? "Device3 OFF" : "Device3 ON");
            break;
        case 4:
            toggleState_4 = !toggleState_4;
            digitalWrite(RelayPin4, toggleState_4 ? HIGH : LOW);
            Serial.println(toggleState_4 ? "Device4 OFF" : "Device4 ON");
            break;
        default:
            break;
    }
}

void with_internet() {
    unsigned long currentMillis = millis();

    if (digitalRead(SwitchPin1) == LOW && (currentMillis - lastDebounceTime1 > debounceDelay)) {
        lastDebounceTime1 = currentMillis;
        relayOnOff(1);
        Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); // Update Button Widget  
    }
    if (digitalRead(SwitchPin2) == LOW && (currentMillis - lastDebounceTime2 > debounceDelay)) {
        lastDebounceTime2 = currentMillis;
        relayOnOff(2);
        Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); // Update Button Widget
    }
    if (digitalRead(SwitchPin3) == LOW && (currentMillis - lastDebounceTime3 > debounceDelay)) {
        lastDebounceTime3 = currentMillis;
        relayOnOff(3);
        Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); // Update Button Widget
    }
    if (digitalRead(SwitchPin4) == LOW && (currentMillis - lastDebounceTime4 > debounceDelay)) {
        lastDebounceTime4 = currentMillis;
        relayOnOff(4);
        Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); // Update Button Widget
    }
}

void without_internet() {
    unsigned long currentMillis = millis();

    if (digitalRead(SwitchPin1) == LOW && (currentMillis - lastDebounceTime1 > debounceDelay)) {
        lastDebounceTime1 = currentMillis;
        relayOnOff(1);      
    }
    if (digitalRead(SwitchPin2) == LOW && (currentMillis - lastDebounceTime2 > debounceDelay)) {
        lastDebounceTime2 = currentMillis;
        relayOnOff(2);
    }
    if (digitalRead(SwitchPin3) == LOW && (currentMillis - lastDebounceTime3 > debounceDelay)) {
        lastDebounceTime3 = currentMillis;
        relayOnOff(3);
    }
    if (digitalRead(SwitchPin4) == LOW && (currentMillis - lastDebounceTime4 > debounceDelay)) {
        lastDebounceTime4 = currentMillis;
        relayOnOff(4);
    }
}

BLYNK_CONNECTED() {
    // Request the latest state from the server
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
    toggleState_1 = param.asInt();
    digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
    toggleState_2 = param.asInt();
    digitalWrite(RelayPin2, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
    toggleState_3 = param.asInt();
    digitalWrite(RelayPin3, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
    toggleState_4 = param.asInt();
    digitalWrite(RelayPin4, toggleState_4);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer
    bool isconnected = Blynk.connected();
    if (!isconnected) {
        wifiFlag = 1;
        digitalWrite(wifiLed, HIGH); // Turn off WiFi LED
    } else {
        wifiFlag = 0;
        digitalWrite(wifiLed, LOW); // Turn on WiFi LED
    }
}

void setup() {
    Serial.begin(9600);

    pinMode(RelayPin1, OUTPUT);
    pinMode(RelayPin2, OUTPUT);
    pinMode(RelayPin3, OUTPUT);
    pinMode(RelayPin4, OUTPUT);

    pinMode(wifiLed, OUTPUT);

    pinMode(SwitchPin1, INPUT_PULLUP);
    pinMode(SwitchPin2, INPUT_PULLUP);
    pinMode(SwitchPin3, INPUT_PULLUP);
    pinMode(SwitchPin4, INPUT_PULLUP);

    // During Starting, all Relays should TURN OFF
    digitalWrite(RelayPin1, toggleState_1);
    digitalWrite(RelayPin2, toggleState_2);
    digitalWrite(RelayPin3, toggleState_3);
    digitalWrite(RelayPin4, toggleState_4);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
    Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {  
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Not Connected");
    } else {
        Serial.println("WiFi Connected");
        Blynk.run();
    }

    timer.run(); // Initiates SimpleTimer
    if (wifiFlag == 0) {
        with_internet();
    } else {
        without_internet();
    }
}
