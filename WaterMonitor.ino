/*
 * Project: IoT Water Level & Pressure Monitor
 * Author: Gemini
 * Date: 2025
 * * Hardware Requirements:
 * 1. Arduino Board (Uno/Nano/Mega)
 * 2. Ultrasonic Sensor (HC-SR04) for Water Level
 * 3. Analog Pressure Transducer (Signal pin to A0)
 * 4. Active Buzzer
 * * Pin Mapping:
 * - Pressure Sensor Signal: A0
 * - Ultrasonic Trig:        Pin 9
 * - Ultrasonic Echo:        Pin 10
 * - Buzzer Positive:        Pin 8
 */

// --- Pin Configuration ---
const int pressurePin = A0;    // Analog input for pressure sensor
const int trigPin = 9;         // Digital output for Ultrasonic Trig
const int echoPin = 10;        // Digital input for Ultrasonic Echo
const int buzzerPin = 8;       // Digital output for Buzzer

// --- Alarm Thresholds ---
// ADJUST THESE VALUES AFTER TESTING
const int MAX_PRESSURE_THRESHOLD = 800; // Analog value (0-1023)
const int MIN_WATER_DISTANCE_CM = 5;    // Distance in cm (Lower value = Tank is full)

// --- Variables ---
long duration;
int distanceCm;
int pressureValue;

void setup() {
  // Initialize Serial Monitor for data viewing
  Serial.begin(9600);
  
  // Set Pin Modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pressurePin, INPUT);
  
  // specific startup sequence to test buzzer
  Serial.println("--- System Initializing ---");
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  Serial.println("--- Sensor Monitoring Started ---");
}

void loop() {
  // --- 1. Read Water Level (Ultrasonic Sensor) ---
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance (Speed of sound is 0.034 cm/us)
  distanceCm = duration * 0.034 / 2;


  // --- 2. Read Water Pressure ---
  pressureValue = analogRead(pressurePin);


  // --- 3. Safety Logic & Alarm ---
  // Alarm triggers if: Pressure is too HIGH -OR- Water is too CLOSE (Full)
  if (pressureValue > MAX_PRESSURE_THRESHOLD || distanceCm < MIN_WATER_DISTANCE_CM) {
    digitalWrite(buzzerPin, HIGH); // Alarm ON
    Serial.print("!! ALERT !! ");
  } else {
    digitalWrite(buzzerPin, LOW);  // Alarm OFF
  }


  // --- 4. Serial Output (Plotter Compatible) ---
  // Prints values to Serial Monitor. 
  Serial.print("Pressure_Raw:");
  Serial.print(pressureValue);
  Serial.print("   "); // Tab space
  Serial.print("Water_Distance_cm:");
  Serial.println(distanceCm);

  // Small delay for stability
  delay(500); 
}