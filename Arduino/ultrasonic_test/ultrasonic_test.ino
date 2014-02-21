/* 
 * Ultrasonic Test, based off of IR Distance Test
 * http://www.maxbotix.com/documents/MB1030_Datasheet.pdf
 */
 
int sonarPin = 2;
int sonarReading;
 
void setup(void) {
  Serial.begin(9600);   
}
 
void loop(void) {
  sonarReading = 0;
  for (int i=0; i<8; i++) {
    sonarReading += analogRead(sonarPin);
    delay(50);
  }  
  sonarReading /= 8;
  
  Serial.print("Sonar Distance reading = ");
  Serial.print(sonarReading / 2.54); // This sensor gives us cm
  Serial.println("in");
  delay(500);
}
