#include <SPI.h>        // SPI communication library (used by nRF24L01)
#include <nRF24L01.h>  // nRF24L01 register definitions
#include <RF24.h>      // Main RF24 control library
#include <Servo.h>     // Used to generate servo-style PWM (1000–2000 µs)

const uint64_t pipeIn = 0xE8E8F0F0E1LL;  // 5-byte address of the RX pipe (must match TX address)

RF24 radio(9, 10);  // CE = 9, CSN = 10 → SPI control pins for the nRF24L01

struct Received_data {  // Payload structure (MAX ≤ 32 bytes for nRF24)
  byte ch1;  // Channel 1 → Throttle
  byte ch2;  // Channel 2 → Roll
  byte ch3;  // Channel 3 → Pitch
  byte ch4;  // Channel 4 → Yaw
  byte ch5;  // Channel 5 → Aux / Switch
};

Received_data received_data;  // Object that will store incoming RF data

Servo channel_1;  // Servo object for channel 1 PWM output
Servo channel_2;  // Servo object for channel 2 PWM output
Servo channel_3;  // Servo object for channel 3 PWM output
Servo channel_4;  // Servo object for channel 4 PWM output
Servo channel_5;  // Servo object for channel 5 PWM output

int ch1_value = 0;  // PWM value in microseconds for channel 1
int ch2_value = 0;  // PWM value in microseconds for channel 2
int ch3_value = 0;  // PWM value in microseconds for channel 3
int ch4_value = 0;  // PWM value in microseconds for channel 4
int ch5_value = 0;  // PWM value in microseconds for channel 5

unsigned long lastRecvTime = 0;  // Stores last time when valid RF packet was received

void reset_the_Data() {  
  received_data.ch1 = 0;    // Throttle LOW (safety)
  received_data.ch2 = 127;  // Center position
  received_data.ch3 = 127;  // Center position
  received_data.ch4 = 127;  // Center position
  received_data.ch5 = 0;    // AUX low
}

void setup() {

  channel_1.attach(2);  // Attach PWM output pin for channel 1
  channel_2.attach(3);  // Attach PWM output pin for channel 2
  channel_3.attach(4);  // Attach PWM output pin for channel 3
  channel_4.attach(5);  // Attach PWM output pin for channel 4
  channel_5.attach(6);  // Attach PWM output pin for channel 5

  reset_the_Data();  // Initialize channels with safe values

  radio.begin();  // Initialize nRF24L01
  radio.setAutoAck(false);  // Disable auto acknowledgment → lower latency
  radio.setDataRate(RF24_250KBPS);  // Lowest speed = max range & stability
  radio.openReadingPipe(1, pipeIn);  // Open RX pipe with defined address
  radio.startListening();  // Set module as RECEIVER

  Serial.begin(9600);  // Start serial for debugging
}

void receive_the_data() {
  while (radio.available()) {  // Check if packet available in FIFO

    radio.read(&received_data, sizeof(Received_data));  // Read full struct
    lastRecvTime = millis();  // Update last successful receive time
    Serial.print("on1");  // Debug → packet received

  }
}

void loop() {

  receive_the_data();  // Continuously check for RF data

  unsigned long now = millis();  // Current time

  if (now - lastRecvTime > 1000) {  // If no signal for 1 second → FAILSAFE
    reset_the_Data();  // Set all channels to safe state
  }

  Serial.println("on2");  // Loop running debug message

  ch1_value = map(received_data.ch1, 0, 255, 1000, 2000);  // Map to PWM range
  ch2_value = map(received_data.ch2, 0, 255, 1000, 2000);  // Map to PWM range
  ch3_value = map(received_data.ch3, 0, 255, 1000, 2000);  // Map to PWM range
  ch4_value = map(received_data.ch4, 0, 255, 1000, 2000);  // Map to PWM range
  ch5_value = map(received_data.ch5, 0, 255, 1000, 2000);  // Map to PWM range

  channel_1.writeMicroseconds(ch1_value);  // Output PWM to channel 1
  channel_2.writeMicroseconds(ch2_value);  // Output PWM to channel 2
  channel_3.writeMicroseconds(ch3_value);  // Output PWM to channel 3
  channel_4.writeMicroseconds(ch4_value);  // Output PWM to channel 4
  channel_5.writeMicroseconds(ch5_value);  // Output PWM to channel 5
}
