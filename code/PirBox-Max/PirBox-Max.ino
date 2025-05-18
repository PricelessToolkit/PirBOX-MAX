/***************************************************************
 * PirBox_MAX by PricelessToolkit
 ***************************************************************/

#include "config.h"
#include <SPI.h>
#include "Ra01S.h"
#include <ArduinoJson.h>



// -------------------- PIN ASSIGNMENTS -------------------- //
#define OFF_PIN          PIN_PC0  // Pull HIGH for Power OFF
#define BTN_PIN          PIN_PB1  // Push button input
#define PIR_PIN          PIN_PA5  // Motion sensor input
#define RSW_PIN_1        PIN_PB0  // Reed switch 1 input
#define RSW_PIN_2        PIN_PC1  // Reed switch 2 input
#define RELAY_PIN_1      PIN_PA7  // Relay 1
#define RELAY_PIN_2      PIN_PB4  // Relay 2

// RA-01SH Pins
#define RADIO_RESET_PIN  PIN_PA6
#define RADIO_NSS_PIN    PIN_PA4
#define RADIO_BUSY_PIN   PIN_PC3
#define RADIO_DIO1_PIN   PIN_PC2

SX126x lora(RADIO_NSS_PIN, RADIO_RESET_PIN, RADIO_BUSY_PIN);

// -------------------- GLOBAL VARIABLES -------------------- //
// Flag to indicate a sensor interrupt occurred (set in ISR)
volatile bool sensorTriggered = false;
// Timestamp of the last sensor event (updated in ISR)
volatile unsigned long lastInterruptTime = 0;
// Flag to indicate that powerOFF is pending
bool shouldPowerOff = false;

// Global flag to force sending on the very first run
bool firstRun = true;

// Global variables to track when to turn off each relay (non-blocking)
unsigned long relay1OffTime = 0;
unsigned long relay2OffTime = 0;

unsigned long powerTimerStart = 0;
bool timerStarted = false;

// -------------------- INTERRUPT SERVICE ROUTINE -------------------- //
void sensorISR() {
  sensorTriggered = true;
  lastInterruptTime = millis();
}

// -------------------- FUNCTIONS -------------------- //
void initLoRaRadio() {
  int16_t ret = lora.begin(BAND, TX_OUTPUT_POWER);
  if (ret != ERR_NONE) {
    // Serial.println(F("LoRa init failed!"));
    while (1);
  }

  // Configure LoRa parameters
  lora.LoRaConfig(
    LORA_SPREADING_FACTOR,
    LORA_BANDWIDTH,
    LORA_CODINGRATE,
    LORA_PREAMBLE_LENGTH,
    LORA_PAYLOADLENGTH,
    LORA_CRC_ON,
    false,  // No IQ inversion
    LORA_SYNC_WORD
  );

  // Serial.println(F("LoRa ready to go!"));
}

void powerOFF() {
  delay(100);
  // Serial.println(F("Sleeping"));
  delay(10);
  digitalWrite(OFF_PIN, HIGH); // Powering off
}

// -------------------- BATTERY MEASUREMENT -------------------- //
int batt() {
  float totalVolts = 0;
  for (int i = 0; i < 5; i++) {
    float volts = analogReadEnh(PIN_PB5, 12) * (2.5 / 4096) * 2; // Voltage divider calculation
    totalVolts += volts;
    //delay(2);
  }
  float averageVolts = totalVolts / 5;
  float percentage = ((averageVolts - 3.2) / (4.5 - 3.3)) * 100;
  percentage = constrain(percentage, 0, 100);
  return (int)percentage;
}

void updateRelayStates() {
  unsigned long currentMillis = millis();
  if (relay1OffTime != 0 && currentMillis >= relay1OffTime) {
    digitalWrite(RELAY_PIN_1, LOW);
    relay1OffTime = 0;
    // Serial.println(F("Relay 1 turned off"));
  }
  if (relay2OffTime != 0 && currentMillis >= relay2OffTime) {
    digitalWrite(RELAY_PIN_2, LOW);
    relay2OffTime = 0;
    // Serial.println(F("Relay 2 turned off"));
  }
}

// This function receives data via LoRa, parses JSON content, and controls the relays.
void ComReceive() {
  uint8_t rxData[255];
  uint8_t rxLen = lora.Receive(rxData, 255);
  if (rxLen > 0) {
    String receivedMsg = "";
    for (int i = 0; i < rxLen; i++) {
      receivedMsg += (char)rxData[i];
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, receivedMsg);
    if (error) {
      return;
    }

    const char* id = doc["id"];
    const char* key = doc["k"];
    String comStr = doc["com"].as<String>();

    // Check if id and key match
    if (String(id) == NODE_NAME && String(key) == GATEWAY_KEY) {
      // Handle command
      if (comStr == "10" || comStr == "11") {
        digitalWrite(RELAY_PIN_1, HIGH);
        relay1OffTime = millis() + RelayOn_Time * 1000;
      } else {
        digitalWrite(RELAY_PIN_1, LOW);  // turn OFF if not asked
      }

      if (comStr == "01" || comStr == "11") {
        digitalWrite(RELAY_PIN_2, HIGH);
        relay2OffTime = millis() + RelayOn_Time * 1000;
      } else {
        digitalWrite(RELAY_PIN_2, LOW);  // turn OFF if not asked
      }

      // Build ACK (compact 2-bit style)
      String r1 = digitalRead(RELAY_PIN_1) == HIGH ? "1" : "0";
      String r2 = digitalRead(RELAY_PIN_2) == HIGH ? "1" : "0";
      String combined = r1 + r2;

      String ack = "{\"k\":\"" + String(GATEWAY_KEY) + "\",\"id\":\"" + String(NODE_NAME) + "\",\"rw\":\"" + combined + "\"}";

      const char* ackStr = ack.c_str();
      lora.Send((uint8_t*)ackStr, ack.length(), SX126x_TXMODE_SYNC);
    }

  }
}


// -------------------- SETUP -------------------- //
void setup() {
  // Set up power off pin
  pinMode(OFF_PIN, OUTPUT);
  digitalWrite(OFF_PIN, LOW);
  
  // Set up sensor pins as inputs
  pinMode(BTN_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(RSW_PIN_1, INPUT);
  pinMode(RSW_PIN_2, INPUT);
  
  // Set up relay outputs
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, HIGH);
  
  pinMode(RADIO_DIO1_PIN, INPUT);
  delay(10);
  
  Serial.begin(9600);
  // Serial.println(F("Serial Debugging Enabled!"));

  // Initialize LoRa radio
  initLoRaRadio();

  // set reference to the desired voltage
  analogReference(INTERNAL2V5);
  //analogReference(VDD);

  // -------------------- ATTACH INTERRUPTS -------------------- //
  // Attach interrupts to sensor pins (trigger on any state change)
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), sensorISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), sensorISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RSW_PIN_1), sensorISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RSW_PIN_2), sensorISR, CHANGE);
}



// -------------------- LOOP -------------------- //
void loop() {
  // -------------------- SENSOR TRIGGER HANDLING -------------------- //
  if (sensorTriggered || firstRun) {
    sensorTriggered = false;

    // Read sensor states
    int currentBtn  = digitalRead(BTN_PIN);
    int currentPIR  = digitalRead(PIR_PIN);
    int currentRSW1 = digitalRead(RSW_PIN_1);
    int currentRSW2 = digitalRead(RSW_PIN_2);

    firstRun = false;

    // Build JSON payload
    String payload = "{\"k\":\"" + String(GATEWAY_KEY)
                   + "\",\"id\":\"" + String(NODE_NAME) + "\""
                   + ",\"bt\":\"" + String(currentBtn == HIGH ? "off" : "on") + "\""
                   + ",\"m\":\""  + String(currentPIR == HIGH ? "on" : "off") + "\""
                   + ",\"dr\":\"" + String(currentRSW1 == HIGH ? "on" : "off") + "\""
                   + ",\"wd\":\"" + String(currentRSW2 == HIGH ? "on" : "off") + "\""
                   + ",\"b\":" + String(batt()) + "}";

    // Send LoRa packet
    const char* cstrPayload = payload.c_str();
    uint8_t length = payload.length();

    if (lora.Send((uint8_t*)cstrPayload, length, SX126x_TXMODE_SYNC)) {
      delay(20);
    }

    // Decide what to do after sending
    if (String(Power) == "Battery") {
      if (String(TwoWayCom) == "True") {
        powerTimerStart = millis();   // Start 3s wait
        timerStarted = true;
      } else if (String(TwoWayCom) == "False") {
        powerOFF();  // Shutdown immediately
      }
    }
  }

  // -------------------- GENERAL PROCESSING -------------------- //

  // Always process communication and relays
  ComReceive();
  updateRelayStates();

  // Handle countdown shutdown if waiting (Battery + TwoWayCom = True)
  if (timerStarted && String(Power) == "Battery" && String(TwoWayCom) == "True") {
    if (millis() - powerTimerStart >= (KeepPowerON_Time * 1000UL)) {
      powerOFF();
    }
  }

  // If running on external power, reset any power-off timer
  if (String(Power) == "External") {
    timerStarted = false;
  }

  // Add any other non-blocking background logic here if needed
}

