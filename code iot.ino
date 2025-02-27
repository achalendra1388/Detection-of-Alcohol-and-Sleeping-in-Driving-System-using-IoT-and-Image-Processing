// Pin Definitions
const int alcoholSensorPin = A0;       // Analog pin for MQ3 alcohol sensor
const int leftRelayPin = 3;            // Relay control pin for Left button on remote
const int forwardRelayPin = 4;         // Relay control pin for Forward button on remote
const int drowsinessInputPin = 4;      // Pin to receive drowsiness signal from Arduino (Drowsiness Detection) via D5
const int waterMotorSignalPin = 6;     // Pin to send water motor signal to Arduino (Drowsiness Detection)
const int buzzerSignalPin = 6;         // Pin to send buzzer signal to Arduino (Drowsiness Detection)

// Threshold for Alcohol Detection
int alcoholThreshold = 400;            // Adjust this based on sensor calibration

void setup() {
  Serial.begin(9600);                  // Initialize serial communication
  pinMode(alcoholSensorPin, INPUT);
  pinMode(leftRelayPin, OUTPUT);
  pinMode(forwardRelayPin, OUTPUT);
  pinMode(drowsinessInputPin, INPUT);
  pinMode(waterMotorSignalPin, OUTPUT);
  pinMode(buzzerSignalPin, OUTPUT);

  // Start with relays and components off
  digitalWrite(leftRelayPin, LOW);
  digitalWrite(forwardRelayPin, LOW);
  digitalWrite(waterMotorSignalPin, LOW);
  digitalWrite(buzzerSignalPin, LOW);

  // Initial check to ensure "no alcohol detected" at startup
  while (analogRead(alcoholSensorPin) > alcoholThreshold) {
    Serial.println("Alcohol detected on startup. Waiting until no alcohol is detected...");
    delay(1000);                       // Wait and check again every second
  }
  Serial.println("No alcohol detected. Car is ready to start.");
}

void loop() {
  int alcoholLevel = analogRead(alcoholSensorPin);  // Read alcohol sensor value
  int drowsinessSignal = digitalRead(drowsinessInputPin);  // Read signal from Arduino (Drowsiness Detection)

  // Display the alcohol level on the Serial Monitor
  Serial.print("Alcohol Level: ");
  Serial.println(alcoholLevel);

  // Handle alcohol detection
  if (alcoholLevel > alcoholThreshold) {
    Serial.println("Alcohol Detected - Activating left turn and stopping forward movement...");
    
    // Activate relays for self-parking
    digitalWrite(leftRelayPin, HIGH);        // Simulate Left button press
    digitalWrite(forwardRelayPin, HIGH);     // Simulate Forward button press
    delay(2000);                             // Keep both buttons "pressed" for 2 seconds

    Serial.println("Self-parking complete. Stopping the car.");
    digitalWrite(leftRelayPin, LOW);         // Turn off Left button relay
    digitalWrite(forwardRelayPin, HIGH);     // Keep Forward button relay on to prevent movement

    // Keep the car inactive after parking
    while (true) {
      delay(1000);                           // Infinite delay to keep the car stopped
    }
  }

  // Handle drowsiness detection signal from Arduino (Drowsiness Detection)
  if (drowsinessSignal == HIGH) {
    Serial.println("Drowsiness Detected - Sending signals to Arduino (Drowsiness Detection)...");

    // Send signals to Arduino (Drowsiness Detection) to handle water motor, buzzer, and engine
    digitalWrite(waterMotorSignalPin, HIGH);
    digitalWrite(buzzerSignalPin, HIGH);
    delay(2000);                              // Signal duration

    // Turn off signals
    digitalWrite(waterMotorSignalPin, LOW);
    digitalWrite(buzzerSignalPin, LOW);
  }

  delay(100);  // Small delay to stabilize readings
}
