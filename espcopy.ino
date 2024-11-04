// Template ID, Name & Auth Token

#define BLYNK_PRINT Serial            
#include <BlynkSimpleEsp8266.h> 

// Define the GPIO connected with Relays
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 14 //D5
#define RelayPin4 12 //D6

#define wifiLed 16   //D0

#define VPIN_BUTTON_1 V1 
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3 
#define VPIN_BUTTON_4 V4

int toggleState_1 = 1; // Initially OFF
int toggleState_2 = 1; // Initially OFF
int toggleState_3 = 1; // Initially OFF
int toggleState_4 = 1; // Initially OFF

#define WIFI_SSID "FreeLink"             
#define WIFI_PASS "chaddi.com"

BlynkTimer timer;

void relayOnOff(int relay) {
    switch(relay) {
      case 1: 
             if(toggleState_1 == 1) {
              digitalWrite(RelayPin1, LOW); 
              toggleState_1 = 0;
              Serial.println("Device1 ON");
             } else {
              digitalWrite(RelayPin1, HIGH);
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
             }
             break;
      case 2: 
             if(toggleState_2 == 1) {
              digitalWrite(RelayPin2, LOW);
              toggleState_2 = 0;
              Serial.println("Device2 ON");
             } else {
              digitalWrite(RelayPin2, HIGH);
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
             }
             break;
      case 3: 
             if(toggleState_3 == 1) {
              digitalWrite(RelayPin3, LOW);
              toggleState_3 = 0;
              Serial.println("Device3 ON");
             } else {
              digitalWrite(RelayPin3, HIGH);
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
             }
             break;
      case 4: 
             if(toggleState_4 == 1) {
              digitalWrite(RelayPin4, LOW);
              toggleState_4 = 0;
              Serial.println("Device4 ON");
             } else {
              digitalWrite(RelayPin4, HIGH);
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
             }
             break;
      default: break;      
    }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nStarting Home Automation System...");

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(wifiLed, OUTPUT);

  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  digitalWrite(wifiLed, HIGH);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkConnection);
  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {  
  Blynk.run();
  timer.run();
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
  Serial.println(toggleState_1 ? "Device1 OFF" : "Device1 ON");
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
  Serial.println(toggleState_2 ? "Device2 OFF" : "Device2 ON");
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
  Serial.println(toggleState_3 ? "Device3 OFF" : "Device3 ON");
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
  Serial.println(toggleState_4 ? "Device4 OFF" : "Device4 ON");
}

void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi NOT CONNECTED");
  } else {
    Serial.println("WiFi CONNECTED!");
  }
}
