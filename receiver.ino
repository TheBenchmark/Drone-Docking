/*
 * Project Name: Drone Docking
 * Author: Bryan Rains
 * Date: 2/5/2025
 * Description:
 *     This is for the receiver on the drone to identify position relative to dock and decode what 
 *     directions should be sent via Mavlink to the drone for correction (if needed). 
 * Hardware:
 *     Arduino Pro Mini 5V, VS1838 Infrared Receiver (38kHz carrier)
 *
 * Revision History:
 *    
 * To Do:
 *     2/8/2025 transmit and receive working but received codes do not match sent codes
 *     Setup finite state machine for every combination of received codes
 *     Implement MAVLINK communication to the flight controller
 *     Send pitch/roll or other commands over MAVLINK depending on state
 */

#include <IRremote.hpp>

// Define the IR receiver pin (adjust if needed)
const uint8_t RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);

void setup() {
  Serial.begin(9600);
  // Wait for Serial Monitor to open (if needed, e.g., on certain boards)
  while (!Serial) {
    ;
  }
  
  // Start the IR receiver.
  irrecv.enableIRIn();
  Serial.println("IR Receiver active. Waiting for signals...");
}

void loop() {
  // Call decode() without parameters.
  if (irrecv.decode()) {
    // Print the protocol, address, and command.
    Serial.print("Protocol: ");
    Serial.println(irrecv.decodedIRData.protocol);
    
    Serial.print("Address: 0x");
    Serial.println(irrecv.decodedIRData.address, HEX);
    
    Serial.print("Command: 0x");
    Serial.println(irrecv.decodedIRData.command, HEX);
    
    Serial.println("-------------------------");
    
    // Ready to receive the next signal.
    irrecv.resume();
  }
}
