#include <SPI.h>        // Required for SPI communication
#include <nRF24L01.h>  // nRF24L01 register definitions
#include <RF24.h>      // Main RF24 library

// Address (pipe) through which data will be transmitted
// Must be the same on the receiver side
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; 

// Create RF24 object
// CE -> pin 9, CSN -> pin 10
RF24 radio(9, 10);  

// -------- DATA STRUCTURE --------
// Total size must be <= 32 bytes for nRF24L01
struct Data_to_be_sent {
  byte ch1;   // Channel 1 (Joystick / potentiometer 1)
  byte ch2;   // Channel 2
  byte ch3;   // Channel 3
  byte ch4;   // Channel 4
  byte ch5;   // Channel 5 (can be button / extra control)
};

// Create a variable of the structure
Data_to_be_sent sent_data;

void setup()
{
  radio.begin();                 // Initialize nRF24L01 module

  radio.setAutoAck(false);      // Disable auto acknowledgment (faster, but no confirmation)
  
  radio.setDataRate(RF24_250KBPS); // Set lower data rate for better range & stability
  
  radio.openWritingPipe(my_radio_pipe); // Set the transmission address

  // -------- INITIAL DEFAULT VALUES --------
  // Neutral position for joysticks
  sent_data.ch1 = 127;
  sent_data.ch2 = 127;
  sent_data.ch3 = 127;
  sent_data.ch4 = 127;

  // Channel 5 default LOW (for switch/button type use)
  sent_data.ch5 = 0;
}

void loop()
{
  // Read analog values (0–1023) and convert to byte range (0–255)

  // Reverse mapping for ch1 (useful if joystick direction is inverted)
  sent_data.ch1 = map(analogRead(A1), 0, 1024, 255, 0);

  // Normal mapping
  sent_data.ch2 = map(analogRead(A0), 0, 1024, 0, 255);
  sent_data.ch3 = map(analogRead(A2), 0, 1024, 0, 255);
  sent_data.ch4 = map(analogRead(A3), 0, 1024, 0, 255);
  sent_data.ch5 = map(analogRead(A4), 0, 1024, 0, 255);

  // Transmit the entire structure in one packet
  // Receiver must use the SAME structure
  radio.write(&sent_data, sizeof(Data_to_be_sent));
}
