// Snake on the plate
// Apparat AS 
// Innevasjonsdag, 29.august.2019

#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        D5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 300 // 15x20

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

// plate with NUMPIXELS pixels
char plate[NUMPIXELS];
String string_plate = "";

const char* ssid = "wifi-ssid-here";
const char* password =  "wifi-password-here";
const char* server = "base-url-here/screen";

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

//  Serial.begin(9600);

  WiFi.begin(ssid, password);

  // Connecting
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  // Read the plate from the server
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
  
    HTTPClient http;
    http.begin(server);
    
    int httpCode = http.GET();

    string_plate = http.getString();
    
    string_plate.toCharArray(plate, NUMPIXELS+1);
    
    http.end();

//    Serial.println(plate);

    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

      switch (plate[i]) {
        // plate background
        case '0':
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          break;
        // snake
        case '1':
          pixels.setPixelColor(i, pixels.Color(0, 255, 0));
          break;
        // apple
        case '2':
          pixels.setPixelColor(i, pixels.Color(255, 0, 0));
          break;
        // end of the array
        case '\n':
          break;
        default:
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          break;
      }

    }
//    Serial.println("Draw");
    
    pixels.show();   // Send the updated pixel colors to the hardware.
    
    delay(DELAYVAL); // Pause before next pass through loop
  }
}
