#define BLYNK_TEMPLATE_ID "IMPL6w817Fd5m"
#define BLYNK_TEMPLATE_NAME "nodemcu"
#define BLYNK_AUTH_TOKEN "f9Q1B40pu9X3zFdtHA2V722GsQTy3fvG"

#include <BlynkSimpleEsp8266.h>

// define the GPIO connected with Relays and switches
#define RelayPin1 5   //D1
#define RelayPin2 4   //D2
#define RelayPin3 14  //D5
#define RelayPin4 12  //D6

#define SwitchPin1 2  //D4 (Reassigned from SD3)
#define SwitchPin2 0  //D3
#define SwitchPin3 13 //D7
#define SwitchPin4 16 //D0 (Reassigned from RX)

#define wifiLed   3   //RX used for WiFi status LED

#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3
#define VPIN_BUTTON_4    V4

int toggleState_1 = 1; 
int toggleState_2 = 1; 
int toggleState_3 = 1; 
int toggleState_4 = 1; 

int wifiFlag = 0;

#define WIFI_SSID "WiFi"
#define WIFI_PASS "12345678"

BlynkTimer timer;

// Debounce variables
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50;

// Function prototypes
void reconnectWiFi();
void reconnectBlynk();

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

void handleSwitch(int switchPin, int relay, int switchIndex) {
    if (digitalRead(switchPin) == LOW) {
        unsigned long currentTime = millis();
        if (currentTime - lastDebounceTime[switchIndex] > debounceDelay) {
            lastDebounceTime[switchIndex] = currentTime;
            relayOnOff(relay);
            if (wifiFlag == 0) {
                Blynk.virtualWrite(VPIN_BUTTON_1 + relay - 1, toggleState_1 + relay - 1);
            }
        }
    }
}

void with_internet() {
    handleSwitch(SwitchPin1, 1, 0);
    handleSwitch(SwitchPin2, 2, 1);
    handleSwitch(SwitchPin3, 3, 2);
    handleSwitch(SwitchPin4, 4, 3);
}

void without_internet() {
    handleSwitch(SwitchPin1, 1, 0);
    handleSwitch(SwitchPin2, 2, 1);
    handleSwitch(SwitchPin3, 3, 2);
    handleSwitch(SwitchPin4, 4, 3);
}

BLYNK_CONNECTED() {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
    toggleState_1 = param.asInt();
    digitalWrite(RelayPin1, toggleState_1 ? HIGH : LOW);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
    toggleState_2 = param.asInt();
    digitalWrite(RelayPin2, toggleState_2 ? HIGH : LOW);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
    toggleState_3 = param.asInt();
    digitalWrite(RelayPin3, toggleState_3 ? HIGH : LOW);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
    toggleState_4 = param.asInt();
    digitalWrite(RelayPin4, toggleState_4 ? HIGH : LOW);
}

void checkBlynkStatus() {
    if (!Blynk.connected()) {
        wifiFlag = 1;
        digitalWrite(wifiLed, HIGH);
    } else {
        wifiFlag = 0;
        digitalWrite(wifiLed, LOW);
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

    // Initialize relays as off
    digitalWrite(RelayPin1, HIGH);
    digitalWrite(RelayPin2, HIGH);
    digitalWrite(RelayPin3, HIGH);
    digitalWrite(RelayPin4, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    timer.setInterval(3000L, checkBlynkStatus);
    Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {
    reconnectWiFi();
    reconnectBlynk();

    if (WiFi.status() == WL_CONNECTED) {
        Blynk.run();
    } else {
        Serial.println("WiFi Not Connected");
    }

    timer.run();
    if (wifiFlag == 0) {
        with_internet();
    } else {
        without_internet();
    }
}

void reconnectWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
}

void reconnectBlynk() {
    if (!Blynk.connected()) {
        Blynk.connect();
    }
}
