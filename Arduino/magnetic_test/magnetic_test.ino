/* 
 * Magnetic Sensor Test, based off of photocell test
 */
 
int magneticPin = 3;     // the cell and 10K pulldown are connected to a0
int magneticReading;     // the analog reading from the analog resistor divider
 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
}
 
void loop(void) {
  magneticReading = analogRead(magneticPin);
 
  Serial.print("Magnetic reading = ");
  float voltage = magneticReading * (5.0 / 1023.0);
  Serial.print(magneticReading);
  Serial.print(" - ");
  Serial.print(voltage);
 
  // We'll have a few threshholds, qualitatively determined
  if (magneticReading < 500) {
    Serial.println(" - Door closed");
  } else {
    Serial.println(" - Door open");
  }
  delay(100);
}
