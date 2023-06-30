#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "SD_MMC.h"
#include "SPI.h"
#include <string.h>

const char* ssid = "nemeth_wifi";
const char* password = "75000000";
const char* ntpServer = "pool.ntp.org";

char credSsid[50];
char credPass[50];



WiFiUDP udp;
NTPClient timeClient(udp, ntpServer);

void setup() {
  Serial.begin(115200);

if (!SD_MMC.begin("/sdcard", true)){
        Serial.println("Failed to mount SD card");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
  File file = SD_MMC.open("/creds.txt"); // Open the file for reading
  if (file) {
    Serial.println("Reading from file:");
    int length = file.available();
    String line0 = file.readStringUntil('"');  // \n character is discarded from buffer
    line0 = file.readStringUntil('"');  // \n character is discarded from buffer
    char ch = file.read();    // read the first character
    String line1 = file.readStringUntil('"');  // \n character is discarded from buffer
    line1 = file.readStringUntil('"');  // \n character is discarded from buffer
    Serial.println(line0);
    Serial.println(line1);
    line0.toCharArray(credSsid, 50);
    line1.toCharArray(credPass, 50);
  }




  WiFi.begin(credSsid, credPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(3600); // Adjust time offset (in seconds) according to your timezone
}

void loop() {
  timeClient.update();

  Serial.print("Current time: ");
  Serial.println(timeClient.getFormattedTime());

  delay(1000);
}