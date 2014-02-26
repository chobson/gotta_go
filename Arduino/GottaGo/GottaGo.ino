/*************************************************** 
 * GottaGo?!
 * This Sketch borrows heavily from Adafruit SendTweet and WebClient
 *
 * Variables to set before running:
 *  WLAN_SSID
 *  WLAN_PASS
 *  host
 *  port
 ****************************************************/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "SSID"           // cannot be longer than 32 characters!
#define WLAN_PASS       "PASSWORD"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
// received before closing the connection.  If you know the server
// you're accessing is quick to respond, you can reduce this value.

// What page to grab!
#define host      "HOST"
#define port      3005
#define endpoint  "/sensor_report"
const char PROGMEM agent[] = "Arduino-GottaGo";

// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr


/**************************************************************************/
/*!
 @brief  Sets up the HW and the CC3000 module (called automatically
 on startup)
 */
/**************************************************************************/

uint32_t ip;
Adafruit_CC3000_Client client;        // For WiFi connections
const unsigned long
dhcpTimeout     = 60L * 1000L, // Max time to wait for address from DHCP
connectTimeout  = 15L * 1000L, // Max time to wait for server connection
responseTimeout = 15L * 1000L; // Max time to wait for data from server

// Sensor information
#define OCCUPIED  "1"
#define AVAILABLE "0"
// tokens
#define photocell  "photocell"
#define pir        "pir"
#define ir         "ir"
#define sonar      "sonar"
#define magnetic   "magnetic"
// states, true = occupied
boolean photocellState = false;
boolean pirState = false;
boolean irState = false;
boolean sonarState = false;
boolean magneticState = false;

int pirPin = 2;

void setup(void)
{
  pinMode(pirPin, INPUT);     // declare sensor as input
  uint32_t t;
  Serial.begin(115200);
  Serial.print(F("Hello! Initializing CC3000..."));
  if(!cc3000.begin()) hang(F("failed. Check your wiring?"));

  Serial.print(F("OK\r\nConnecting to network..."));
  /* NOTE: Secure connections are not available in 'Tiny' mode! */
  if(!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) hang(F("Failed!"));

  Serial.print(F("OK\r\nRequesting address from DHCP server..."));
  for(t=millis(); !cc3000.checkDHCP() && ((millis() - t) < dhcpTimeout); delay(100));
  if(!cc3000.checkDHCP()) hang(F("failed"));
  Serial.println(F("OK"));

  while(!displayConnectionDetails());
}

void loop(void)
{
  
  boolean reading = readPhotocell();
  if (reading != photocellState) {
    if (reading == true) {
      sendStatus(photocell, OCCUPIED);
    } else {
      sendStatus(photocell, AVAILABLE);
    }
    photocellState = reading;
  }
  
  reading = readPIR();
  if (reading != pirState) {
    if (reading == true) {
      sendStatus(pir, OCCUPIED);
    } else {
      sendStatus(pir, AVAILABLE);
    }
    pirState = reading;
  }
  
  reading = readIR();
  if (reading != irState) {
    if (reading == true) {
      sendStatus(ir, OCCUPIED);
    } else {
      sendStatus(ir, AVAILABLE);
    }
    irState = reading;
  }
  
  reading = readSonar();
  if (reading != sonarState) {
    if (reading == true) {
      sendStatus(sonar, OCCUPIED);
    } else {
      sendStatus(sonar, AVAILABLE);
    }
    sonarState = reading;
  }
  
  reading = readMagnetic();
  if (reading != magneticState) {
    if (reading == true) {
      sendStatus(magnetic, OCCUPIED);
    } else {
      sendStatus(magnetic, AVAILABLE);
    }
    magneticState = reading;
  }

  delay(5000);
}

void sendStatus(char *sensor, char *sensor_status) {
  ip = 0;
  // Try looking up the website's IP address
  Serial.print(host);
  Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(host, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }

  cc3000.printIPdotsRev(ip);

  client = cc3000.connectTCP(ip, port);
  if(client.connected()) { // Success!
    Serial.print(F("OK\r\nIssuing HTTP request..."));
    char msg[] = "test";
    // Unlike the hash prep, parameters in the HTTP request don't require sorting.
    client.fastrprint(F("POST "));
    client.fastrprint(endpoint);
    client.fastrprint(F(" HTTP/1.1\r\nHost: "));
    client.fastrprint(host);
    client.fastrprint(F("\r\nUser-Agent: "));
    client.fastrprint(F2(agent));
    client.fastrprint(F("\r\nConnection: close\r\n"
      "Content-Type: application/x-www-form-urlencoded;charset=UTF-8\r\n"
      "Content-Length: "));
    // 6 => "token="
    // 8 => "&status="
    client.print(14 + encodedLength(sensor) + encodedLength(sensor_status));
    client.fastrprint(F("\r\n\r\ntoken="));
    urlEncode(client, sensor, false, false);
    client.fastrprint(F("&status="));
    urlEncode(client, sensor_status, false, false);

    Serial.print(F("OK\r\nAwaiting response..."));
    /* Read data until either the connection is closed, or the idle timeout is reached. */
    unsigned long lastRead = millis();
    while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
        lastRead = millis();
      }
    }
    client.close();
    Serial.println(F("-------------------------------------"));
  } 
  else { // Couldn't contact server
    Serial.println(F("failed"));
  }
}

// Read sensors

boolean readPhotocell() {
  int photocellPin = 0;
  int photocellReading = 0;
  for (int i=0; i<8; i++) {
    photocellReading += analogRead(photocellPin);  
    delay(50);
  }
  photocellReading /= 8;
  
  if (photocellReading > 700) {
    return true;
  } else {
    return false;
  }
}

boolean readPIR() {
  int pirReading = 0;
  pirReading = digitalRead(pirPin);
  
  if (pirReading == HIGH) {
    return true;
  } else {
    return false;
  }
}

boolean readIR() {
  int irPin = 1;
  int irReading = 0;
  for (int i=0; i<8; i++) {
    irReading += analogRead(irPin);  
    delay(50);
  }
  irReading /= 8;
  float voltage = irReading * (5.0 / 1023.0);
  
  if (voltage > 0.85) { // within 12 inches
    return true;
  } else {
    return false;
  }
}

boolean readSonar() {
  int sonarPin = 2;
  int sonarReading = 0;
  for (int i=0; i<8; i++) {
    sonarReading += analogRead(sonarPin);
    delay(50);
  }  
  sonarReading /= 8;
  sonarReading /= 2.54;
  
  if (sonarReading < 12) {
    return true;
  } else {
    return false;
  }
}

boolean readMagnetic() {
  int magneticPin = 3;
  int magneticReading = 0;

  magneticReading = analogRead(magneticPin);
  
  if (magneticReading < 500) {
    return true;
  } else {
    return false;
  }
}

// On error, print PROGMEM string to serial monitor and stop
void hang(const __FlashStringHelper *str) {
  Serial.println(str);
  for(;;);
}

/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

// Read from client stream with a 5 second timeout.  Although an
// essentially identical method already exists in the Stream() class,
// it's declared private there...so this is a local copy.
int timedRead(void) {
  unsigned long start = millis();
  while((!client.available()) && ((millis() - start) < responseTimeout));
  return client.read();  // -1 on timeout
}

// For URL-encoding functions below
static const char PROGMEM hexChar[] = "0123456789ABCDEF";

// URL-encoding output function for Print class.
// Input from RAM or PROGMEM (flash).  Double-encoding is a weird special
// case for Oauth (encoded strings get encoded a second time).
void urlEncode(
Print      &p,       // EthernetClient, Sha1, etc.
const char *src,     // String to be encoded
boolean     progmem, // If true, string is in PROGMEM (else RAM)
boolean     x2)      // If true, "double encode" parenthesis
{
  uint8_t c;

  while((c = (progmem ? pgm_read_byte(src) : *src))) {
    if(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
      ((c >= '0') && (c <= '9')) || strchr_P(PSTR("-_.~"), c)) {
      p.write(c);
    } 
    else {
      if(x2) p.print("%25");
      else   p.write('%');
      p.write(pgm_read_byte(&hexChar[c >> 4]));
      p.write(pgm_read_byte(&hexChar[c & 15]));
    }
    src++;
  }
}

// Returns would-be length of encoded string, without actually encoding
int encodedLength(char *src) {
  uint8_t c;
  int     len = 0;

  while((c = *src++)) {
    len += (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
      ((c >= '0') && (c <= '9')) || strchr_P(PSTR("-_.~"), c)) ? 1 : 3;
  }

  return len;
}

