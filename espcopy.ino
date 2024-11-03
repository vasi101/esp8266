// Template ID, Name & Auth Token


#define BLYNK_PRINT Serial            
#include <BlynkSimpleEsp8266.h> 

// define the GPIO connected with Relays and switches
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 14 //D5
#define RelayPin4 12 //D6

#define SwitchPin1 10  //SD3
#define SwitchPin2 0   //D3 
#define SwitchPin3 13  //D7
#define SwitchPin4 3   //RX

#define wifiLed   16   //D0

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; //Define integer to remember the toggle state for relay 4

int wifiFlag = 0;

#define WIFI_SSID "FreeLink"             //Enter Wifi Name
#define WIFI_PASS "chaddi.com"    //Enter wifi Password

// Connection monitoring variables
unsigned long previousMillis = 0;
const long interval = 5000;  // Connection status check interval (5 seconds)
int connectionAttempts = 0;

BlynkTimer timer;

void relayOnOff(int relay) {
    switch(relay) {
      case 1: 
             if(toggleState_1 == 1) {
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
             }
             else {
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
             }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1) {
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
             }
             else {
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
             }
             delay(100);
      break;
      case 3: 
             if(toggleState_3 == 1) {
              digitalWrite(RelayPin3, LOW); // turn on relay 3
              toggleState_3 = 0;
              Serial.println("Device3 ON");
             }
             else {
              digitalWrite(RelayPin3, HIGH); // turn off relay 3
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
             }
             delay(100);
      break;
      case 4: 
             if(toggleState_4 == 1) {
              digitalWrite(RelayPin4, LOW); // turn on relay 4
              toggleState_4 = 0;
              Serial.println("Device4 ON");
             }
             else {
              digitalWrite(RelayPin4, HIGH); // turn off relay 4
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
             }
             delay(100);
      break;
      default : break;      
    }
}

void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("╔════════════════════════════════════╗");
    Serial.println("║         WiFi NOT CONNECTED         ║");
    Serial.println("╚════════════════════════════════════╝");
    Serial.print("Attempting to connect to: ");
    Serial.println(WIFI_SSID);
    
    // Print WiFi connection details
    Serial.print("WiFi Status: ");
    switch (WiFi.status()) {
      case WL_NO_SHIELD: Serial.println("NO SHIELD"); break;
      case WL_IDLE_STATUS: Serial.println("IDLE"); break;
      case WL_NO_SSID_AVAIL: Serial.println("NO SSID AVAILABLE"); break;
      case WL_SCAN_COMPLETED: Serial.println("SCAN COMPLETED"); break;
      case WL_CONNECT_FAILED: Serial.println("CONNECT FAILED"); break;
      case WL_CONNECTION_LOST: Serial.println("CONNECTION LOST"); break;
      case WL_DISCONNECTED: Serial.println("DISCONNECTED"); break;
      default: Serial.println("UNKNOWN"); break;
    }
    
    connectionAttempts++;
    if (connectionAttempts > 10) {
      Serial.println("Multiple connection attempts failed. Restarting device...");
      connectionAttempts = 0;
      ESP.restart();
    }
  } else {
    Serial.println("╔════════════════════════════════════╗");
    Serial.println("║         WiFi CONNECTED!            ║");
    Serial.print("║ IP: ");
    Serial.print(WiFi.localIP());
    Serial.println("        ║");
    Serial.print("║ Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm    ║");
    Serial.println("╚════════════════════════════════════╝");

    if (Blynk.connected()) {
      Serial.println("╔════════════════════════════════════╗");
      Serial.println("║         BLYNK CONNECTED!           ║");
      Serial.println("╚════════════════════════════════════╝");
    } else {
      Serial.println("╔════════════════════════════════════╗");
      Serial.println("║       BLYNK NOT CONNECTED          ║");
      Serial.println("╚════════════════════════════════════╝");
    }
  }
}

void with_internet() {
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW) {
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget  
    }
    else if (digitalRead(SwitchPin2) == LOW) {
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin3) == LOW) {
      delay(200);
      relayOnOff(3);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin4) == LOW) {
      delay(200);
      relayOnOff(4);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);   // Update Button Widget
    }
}

void without_internet() {
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW) {
      delay(200);
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW) {
      delay(200);
      relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW) {
      delay(200);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW) {
      delay(200);
      relayOnOff(4);
    }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Serial.println("Blynk Connected - Syncing States");
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

void checkBlynkStatus() {
  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }
}

void setup() {
  Serial.begin(115200);  // Changed to higher baud rate for better debugging
  Serial.println("\n\nStarting Home Automation System...");

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  digitalWrite(wifiLed, HIGH);  // Turn off LED initially

  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {  
  unsigned long currentMillis = millis();
  
  // Check connection status every 5 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkConnection();
  }

  if (WiFi.status() != WL_CONNECTED) {
    // If WiFi is not connected, try to reconnect
    without_internet();
  }
  else {
    Blynk.run();
    timer.run(); // Initiates SimpleTimer
    
    if (wifiFlag == 0)
      with_internet();
    else
      without_internet();
  }
}
