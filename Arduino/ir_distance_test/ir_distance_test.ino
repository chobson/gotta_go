/* 
 * IR Distance Test, based off of photocell test
 * http://sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0a21yk_e.pdf
 * Page 5 for voltage to distance conversions
 */
 
int irPin = 1;
int irReading;
 
void setup(void) {
  Serial.begin(9600);   
}
 
void loop(void) {
  irReading = 0;
  for (int i=0; i<8; i++) {
    irReading += analogRead(irPin);  
    delay(50);
  }
  irReading /= 8;
 
  Serial.print("IR Distance reading = ");
  float voltage = irReading * (5.0 / 1023.0);
  Serial.print(voltage);
 
  if (voltage > 3.0) {
    Serial.println(" - < 2in");
  } else if (voltage > 2.7) {
    Serial.println(" - 2in - 3in");
  } else if (voltage > 2.15) {
    Serial.println(" - 3in - 4in");
  } else if (voltage > 1.8) {
    Serial.println(" - 4in - 5in");
  } else if (voltage > 1.52) {
    Serial.println(" - 5in - 6in");
  } else if (voltage > 1.35) {
    Serial.println(" - 6in - 7in");
  } else if (voltage > 1.2) {
    Serial.println(" - 7in - 8in");
  } else if (voltage > 1.05) {
    Serial.println(" - 8in - 9in");
  } else if (voltage > 0.98) {
    Serial.println(" - 9in - 10in");
  } else if (voltage > 0.9) {
    Serial.println(" - 10in - 11in");
  } else if (voltage > 0.85) {
    Serial.println(" - 11in - 12in");
  } else {
    Serial.println(" - > 12in");
  }
  delay(500);
}
