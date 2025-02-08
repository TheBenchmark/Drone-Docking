// Define the State enum first
enum State {
  WAITING,
  ONLY_9, ONLY_11, ONLY_13, ONLY_15,
  COMBO_9_11, COMBO_11_13, COMBO_13_15, COMBO_9_15,
  COMBO_9_11_13, COMBO_11_13_15, COMBO_9_13_15, COMBO_9_11_15,
  ALL_4
};

// Forward declaration of determineState()
State determineState();

const int irPin = 9;          // IR receiver pin
const unsigned long bitTime = 400;  // Duration of each bit in microseconds
const unsigned long readCycle = (4 * bitTime + 800) * 4 + 1600;  // Total cycle time

volatile unsigned long pulseStart = 0;
volatile unsigned long pulseWidth = 0;
volatile bool newPulse = false;

State currentState = WAITING;

bool received9 = false, received11 = false, received13 = false, received15 = false;

void IR_ISR() {
  if (digitalRead(irPin) == LOW) {
    pulseStart = micros();
  } else {
    pulseWidth = micros() - pulseStart;
    newPulse = true;
  }
}

State determineState() {
  if (received9 && received11 && received13 && received15) return ALL_4;
  if (received9 && received11 && received13) return COMBO_9_11_13;
  if (received11 && received13 && received15) return COMBO_11_13_15;
  if (received9 && received13 && received15) return COMBO_9_13_15;
  if (received9 && received11 && received15) return COMBO_9_11_15;
  if (received9 && received11) return COMBO_9_11;
  if (received11 && received13) return COMBO_11_13;
  if (received13 && received15) return COMBO_13_15;
  if (received9 && received15) return COMBO_9_15;
  if (received9) return ONLY_9;
  if (received11) return ONLY_11;
  if (received13) return ONLY_13;
  if (received15) return ONLY_15;
  return WAITING;
}

void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(irPin), IR_ISR, CHANGE);
}

void loop() {
  received9 = received11 = received13 = received15 = false;

  unsigned long cycleStart = micros();
  while (micros() - cycleStart < readCycle) {
    if (newPulse) {
      noInterrupts();
      unsigned long width = pulseWidth;
      newPulse = false;
      interrupts();

      if (width >= bitTime - 50 && width <= bitTime + 50) {
        // Decode '1'
      } else if (width > 0) {
        // Decode '0'
      }
    }
  }

  currentState = determineState();

   // Print the current state to the Serial Monitor
  Serial.print("Current State: ");
  switch (currentState) {
    case WAITING: Serial.println("WAITING"); break;
    case ONLY_9: Serial.println("ONLY_9"); break;
    case ONLY_11: Serial.println("ONLY_11"); break;
    case ONLY_13: Serial.println("ONLY_13"); break;
    case ONLY_15: Serial.println("ONLY_15"); break;
    case COMBO_9_11: Serial.println("COMBO_9_11"); break;
    case COMBO_11_13: Serial.println("COMBO_11_13"); break;
    case COMBO_13_15: Serial.println("COMBO_13_15"); break;
    case COMBO_9_15: Serial.println("COMBO_9_15"); break;
    case COMBO_9_11_13: Serial.println("COMBO_9_11_13"); break;
    case COMBO_11_13_15: Serial.println("COMBO_11_13_15"); break;
    case COMBO_9_13_15: Serial.println("COMBO_9_13_15"); break;
    case COMBO_9_11_15: Serial.println("COMBO_9_11_15"); break;
    case ALL_4: Serial.println("ALL_4"); break;
  }
  delay(2 * readCycle / 1000);
  currentState = WAITING;
}
