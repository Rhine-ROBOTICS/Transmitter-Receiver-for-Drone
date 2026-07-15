#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include <U8g2lib.h>

// =====================================================
// NRF24L01
// =====================================================

RF24 radio(7, 8);  // CE, CSN

const byte RADIO_ADDRESS[6] = "00001";
const uint16_t PACKET_ID = 24680;

// =====================================================
// OLED
// Page-buffer version saves Arduino Nano RAM
// =====================================================

U8G2_SH1106_128X64_NONAME_1_HW_I2C oled(
  U8G2_R0,
  U8X8_PIN_NONE
);

// =====================================================
// Transmitter inputs
// =====================================================

const byte THROTTLE_PIN = A0;
const byte YAW_PIN      = A1;
const byte ROLL_PIN     = A3;
const byte PITCH_PIN    = A2;

const byte CH5_SWITCH_PIN = 4;

// Change any of these to true if that channel is reversed
const bool REVERSE_ROLL     = false;
const bool REVERSE_PITCH    = true;
const bool REVERSE_THROTTLE = false;
const bool REVERSE_YAW      = true;

// =====================================================
// Radio packet
// Must match the receiver exactly
// =====================================================

struct DataPacket {
  uint16_t packetID;

  uint16_t ch1;
  uint16_t ch2;
  uint16_t ch3;
  uint16_t ch4;
  uint16_t ch5;

  uint32_t counter;
};

DataPacket data = {
  PACKET_ID,
  1500,
  1500,
  1000,
  1500,
  1000,
  0
};

// =====================================================
// Variables
// =====================================================

bool nrfDetected = false;
bool packetAcknowledged = false;

unsigned long lastDisplayTime = 0;
unsigned long lastSerialTime = 0;

// =====================================================
// Read and map one joystick axis
// =====================================================

uint16_t readChannel(byte pin, bool reversed) {
  int rawValue = analogRead(pin);

  int channelValue;

  if (!reversed) {
    channelValue = map(
      rawValue,
      0,
      1023,
      1000,
      2000
    );
  } else {
    channelValue = map(
      rawValue,
      1023,
      0,
      1000,
      2000
    );
  }

  channelValue = constrain(
    channelValue,
    1000,
    2000
  );

  // Small centre dead zone to reduce jitter
  if (channelValue >= 1485 && channelValue <= 1515) {
    channelValue = 1500;
  }

  return channelValue;
}

// =====================================================
// Read all five channels
// =====================================================

void readControls() {
  data.ch1 = readChannel(ROLL_PIN, REVERSE_ROLL);
  data.ch2 = readChannel(PITCH_PIN, REVERSE_PITCH);
  data.ch3 = readChannel(THROTTLE_PIN, REVERSE_THROTTLE);
  data.ch4 = readChannel(YAW_PIN, REVERSE_YAW);

  // INPUT_PULLUP:
  // Switch connected to GND = 2000
  // Switch open = 1000
  data.ch5 =
    digitalRead(CH5_SWITCH_PIN) == LOW
    ? 2000
    : 1000;

  data.packetID = PACKET_ID;
  data.counter++;
}

// =====================================================
// Send radio packet
// =====================================================

void sendPacket() {
  if (!nrfDetected) {
    packetAcknowledged = false;
    return;
  }

  packetAcknowledged = radio.write(
    &data,
    sizeof(data)
  );
}

// =====================================================
// OLED
// =====================================================

void drawDisplay() {
  oled.firstPage();

  do {
    oled.setFont(u8g2_font_5x8_tf);

    oled.drawStr(0, 8, "DIY 5CH TRANSMITTER");
    oled.drawHLine(0, 11, 128);

    oled.setCursor(0, 23);
    oled.print("CH1:");
    oled.print(data.ch1);

    oled.setCursor(67, 23);
    oled.print("CH2:");
    oled.print(data.ch2);

    oled.setCursor(0, 35);
    oled.print("CH3:");
    oled.print(data.ch3);

    oled.setCursor(67, 35);
    oled.print("CH4:");
    oled.print(data.ch4);

    oled.setCursor(0, 47);
    oled.print("CH5:");
    oled.print(data.ch5);

    oled.setCursor(0, 61);
    oled.print("LINK: ");

    if (!nrfDetected) {
      oled.print("NO NRF");
    } else if (packetAcknowledged) {
      oled.print("CONNECTED");
    } else {
      oled.print("DISCONNECTED");
    }

  } while (oled.nextPage());
}

// =====================================================
// Setup
// =====================================================

void setup() {
  Serial.begin(115200);

  pinMode(CH5_SWITCH_PIN, INPUT_PULLUP);

  Wire.begin();
  oled.begin();

  oled.firstPage();

  do {
    oled.setFont(u8g2_font_6x12_tf);
    oled.drawStr(19, 20, "5CH TRANSMITTER");
    oled.drawStr(31, 39, "STARTING...");
    oled.drawStr(6, 57, "KEEP THROTTLE LOW");
  } while (oled.nextPage());

  nrfDetected = radio.begin();

  if (nrfDetected) {
    radio.openWritingPipe(RADIO_ADDRESS);

    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(76);
    radio.setCRCLength(RF24_CRC_16);

    radio.setAutoAck(true);
    radio.setRetries(5, 15);

    radio.disableDynamicPayloads();
    radio.setPayloadSize(sizeof(DataPacket));
    radio.flush_tx();

    radio.stopListening();

    Serial.println("Transmitter NRF ready");
  } else {
    Serial.println("ERROR: Transmitter NRF not detected");
  }

  delay(1000);
}

// =====================================================
// Loop
// =====================================================

void loop() {
  readControls();
  sendPacket();

  if (millis() - lastDisplayTime >= 100) {
    lastDisplayTime = millis();
    drawDisplay();
  }

  if (millis() - lastSerialTime >= 250) {
    lastSerialTime = millis();

    Serial.print("CH1:");
    Serial.print(data.ch1);

    Serial.print(" CH2:");
    Serial.print(data.ch2);

    Serial.print(" CH3:");
    Serial.print(data.ch3);

    Serial.print(" CH4:");
    Serial.print(data.ch4);

    Serial.print(" CH5:");
    Serial.print(data.ch5);

    Serial.print(" | Link: ");
    Serial.println(
      packetAcknowledged
      ? "CONNECTED"
      : "DISCONNECTED"
    );
  }

  delay(20);
}