int xAxisPin = A1; // Analog pin for X-axis
int yAxisPin = A0; // Analog pin for Y-axis

const int numReadings = 10;  // Number of readings for averaging
int xReadings[numReadings];  // Array to store X-axis readings
int yReadings[numReadings];  // Array to store Y-axis readings
int xIndex = 0;              // Current index for the averaging array
int yIndex = 0;
int xTotal = 0;              // Total for calculating the average
int yTotal = 0;
int xAverage = 0;            // Averaged X-axis value
int yAverage = 0;
const int deadzone = 1;     // Deadzone size

void setup() {
  Serial.begin(9600);

  // Initialize all readings to center position (e.g., 512 for a 10-bit ADC)
  for (int i = 0; i < numReadings; i++) {
    xReadings[i] = 512;
    yReadings[i] = 512;
  }
}

void loop() {
  // Read raw values
  int xRaw = analogRead(xAxisPin);
  int yRaw = analogRead(yAxisPin);

  // Subtract the last reading and add the new one for X-axis
  xTotal = xTotal - xReadings[xIndex];
  xReadings[xIndex] = xRaw;
  xTotal = xTotal + xReadings[xIndex];
  xIndex = (xIndex + 1) % numReadings;

  // Subtract the last reading and add the new one for Y-axis
  yTotal = yTotal - yReadings[yIndex];
  yReadings[yIndex] = yRaw;
  yTotal = yTotal + yReadings[yIndex];
  yIndex = (yIndex + 1) % numReadings;

  // Calculate the average for X and Y
  xAverage = xTotal / numReadings;
  yAverage = yTotal / numReadings;

  // Apply deadzone
  int xAdjusted = xAverage;
  int yAdjusted = yAverage;
  if (abs(xAdjusted - 512) < deadzone) {
    xAdjusted = 512;  // Set to center if within deadzone
  }
  if (abs(yAdjusted - 512) < deadzone) {
    yAdjusted = 512;  // Set to center if within deadzone
  }

  // Map the adjusted values to a range from -512 to 511 for X and Y axes, centering around 0
  int xMapped = map(xAdjusted, 0, 1023, -511, 511);
  int yMapped = map(yAdjusted, 0, 1023, -511, 511);

  // Apply custom ranges for X and Y
  if (xMapped >= -25 && xMapped <= 0) {
    xMapped = 0;
  }
  if (yMapped >= 0 && yMapped <= 30) {
    yMapped = 0;
  }

  // Print all values to the Serial Monitor
  Serial.print("Raw X: ");
  Serial.print(xRaw);
  Serial.print(" Raw Y: ");
  Serial.print(yRaw);
  Serial.print(" | Smoothed X: ");
  Serial.print(xAverage);
  Serial.print(" Smoothed Y: ");
  Serial.print(yAverage);
  Serial.print(" | Adjusted X: ");
  Serial.print(xAdjusted);
  Serial.print(" Adjusted Y: ");
  Serial.print(yAdjusted);
  Serial.print(" | Mapped X: ");
  Serial.print(xMapped);
  Serial.print(" Mapped Y: ");
  Serial.println(yMapped);

  delay(500);  // Small delay to make the output more readable
}