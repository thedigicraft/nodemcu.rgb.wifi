#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Wifi creds:
const char* ssid = "ssid";
const char* password = "password";

// RGB Light Initialize:
// Pins:
int redPin = 13;    // GPIO 13 | D7 On board.
int greenPin = 12;  // GPIO 14 | D5 On board.
int bluePin = 14;   // GPIO 12 | D6 On board.

// RGB Values:
int redValue = 0; // Initial Red Value.
int greenValue = 0; // Initial Green Value.
int blueValue = 0; // Initial Blue Value.

// Flicker:
int flickerDelay = 0; // Delay value.
int flickerRange = 0; // Color range value.
int flickerDelayRemote = 0; // Delay value from the server.
int flickerRangeRemote = 0; // Color range value from the server.
int flickerRed; // Altered red value to produce flicker effect.
int flickerGreen; // Altered green value to produce flicker effect.
int flickerBlue; // Altered blue value to produce flicker effect.

// If your LED is NOT a common anode; comment the next line out.
#define COMMON_ANODE

int cnt = 0; // Initial loop count.

void setup() {

  Serial.begin(115200); // Start the serial monitor.

  // RGB Light Setup:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("Hello Digital Craft");
  Serial.println("Connecting ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

}

void loop() {

  // After it loops 10 times; check the server for an update:
  if (cnt == 10){

    if (WiFi.status() == WL_CONNECTED) {


      HTTPClient http;
      http.begin("http://dockit.org/candles/data.txt");

      int httpCode = http.GET();

      if (httpCode > 0){

        Serial.println("Reading data: ");

        String payload = http.getString();

        // Parse the data:
        redValue = getValue(payload, ':', 0).toInt();
        greenValue = getValue(payload, ':', 1).toInt();
        blueValue = getValue(payload, ':', 2).toInt();
        flickerRangeRemote = getValue(payload, ':', 3).toInt();
        flickerDelayRemote = getValue(payload, ':', 4).toInt();

        // Print the values to the serial monitor:
        Serial.println("Red:" + String(redValue));
        Serial.println("Green:" + String(greenValue));
        Serial.println("Blue:" + String(blueValue));
        Serial.println("Decay:" + String(flickerRangeRemote));
        Serial.println("Speed:" + String(flickerDelayRemote));

      }else{

        Serial.println("Something baaaaaaad happened!");

      }

      http.end();

    }

    cnt = 0; // Reset loop counter.

  }

  // Get flicker value for Red:
  flickerRange = random(0,flickerRangeRemote); // Random number for color value.
  if ((redValue - flickerRange) < 0) {
    flickerRed = redValue + flickerRange;
  }else{
    flickerRed = redValue - flickerRange;
  }

  // Get flicker value for Green:
  flickerRange = random(0,flickerRangeRemote); // Random number for color value.
  if ((greenValue - flickerRange) < 0) {
    flickerGreen = greenValue + flickerRange;
  }else{
    flickerGreen = greenValue - flickerRange;
  }

  // Get flicker value for Blue:
  flickerRange = random(0,flickerRangeRemote); // Random number for color value.
  if ((blueValue - flickerRange) < 0) {
    flickerBlue = blueValue + flickerRange;
  }else{
    flickerBlue = blueValue - flickerRange;
  }

  // Get flicker delay value:
  flickerDelay = random(20,20+flickerDelayRemote); // Randomize the color flicker/
  setColor(flickerRed, flickerGreen, flickerBlue);  // Send color values to the light.
  delay(flickerDelay); // Apply the delay.
  ++cnt; // Increment the loop count by 1.

}

// Parsing function:
String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Sends color values to LED:
void setColor(int red, int green, int blue) {

  #ifdef COMMON_ANODE
    red = 1024 - red;
    green = 1024 - green;
    blue = 1024 - blue;
  #endif

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);

}
