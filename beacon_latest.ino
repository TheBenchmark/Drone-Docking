/*
 * Project Name: Drone Docking
 * Author: Bryan Rains
 * Date: 2/5/2025
 * Description:
 *     This is for the landing beacon that will be on top of the large drone. 
 * Hardware:
 *     Arduino UNO, IR LEDs, 2n2222 BJTs, current limiting resistors.
 *
 * Revision History:
 *    
 * To Do:
 *     2/8/2025 transmit and receive working but received codes do not match sent codes
 *     Right now using SONY protocol, around 40ms per code send.
 *     Spacing between codes is set by txDelay, we need to see how low we can get that delay before
 *       corrupting data. With the delay and the time for sending data each loop is around 600-700ms
 *       I'd like that time dropped as much as possible.
 */

#include <IRremote.hpp>

// Create an IRsend object.
IRsend irsend;

// Define the IR send pins for the four channels.
const int sendPins[4] = {3, 5, 6, 9};  // Adjust these as needed

// Define the IR codes for each channel (example hex codes).
unsigned long codes[4] = { 0xA90, 0xB90, 0xC90, 0xD90 };

// Sony protocol settings.
const int sonyBits = 12;  // Number of bits in the Sony code

// Delay between transmissions (in milliseconds).
const unsigned long txDelay = 150;

// Variable to track the current channel.
int currentChannel = 0;

void setup() {
  // Initialize each IR send pin as an OUTPUT.
  for (int i = 0; i < 4; i++) {
    pinMode(sendPins[i], OUTPUT);
  }
  
    // Set carrier frequency
  irsend.enableIROut(38);
  
  // Set the initial IR send pin.
  irsend.setSendPin(sendPins[currentChannel]);
}

void loop() {
  // Set the active IR send pin to the one corresponding to the current channel.
  irsend.setSendPin(sendPins[currentChannel]);
  
  // Transmit the Sony IR code for the current channel.
  irsend.sendSony(codes[currentChannel], sonyBits);
  
  // Wait before transmitting the next code.
  delay(txDelay);
  
  // Move to the next channel in a round-robin fashion.
  currentChannel = (currentChannel + 1) % 4;
}
