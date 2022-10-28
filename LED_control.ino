#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

/* *** Pinout ***
 *  Input pin 1: D5
 *  Input pin 2: D6
 *  LED output: D8
*/

// LED setup
#define NUM_LEDS 16  // Change to match the number of addressable LEDs used
#define DATA_PIN D8  // Signal line for LEDs is pin D8
int whichLED = 0; 

// LED positions, to be used in loops. Numbered in order of wiring. 
// Skull eyes (4)
const int waitingPosStart = 12;
const int waitingPosEnd = 16;
// Candy dispense slot (4)
const int dispensingPosStart = 0;
const int dispensingPosEnd = 4;
// Internal (8)
const int displayPosStart = 4;
const int displayPosEnd = 12;

// Define the array of leds
CRGB leds[NUM_LEDS];

// Input pins
const int in1 = D5;
const int in2 = D6;


void setup() 
{
   FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical

  // Listener pins
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
}

void loop() 
{
  // 01
  while(digitalRead(in1) == LOW && digitalRead(in2) == HIGH)
  {
    dispensingLights();
    yield();
  }
  // 10
  while(digitalRead(in1) == HIGH && digitalRead(in2) == LOW)
  {
    errorLights();
    yield();
  }
  // 11
  while(digitalRead(in1) == HIGH && digitalRead(in2) == HIGH)
  {
    waitingLights();
    yield();
  }
}

void waitingLights()
{

  // Dim display lights
  for(int i = displayPosStart; i < displayPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 85);
  }
  // Disable dispensing lights
  for(int i = dispensingPosStart; i < dispensingPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 0);
  }
  // Brighten waiting lights
  for(int i = waitingPosStart; i < waitingPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 50);
  }
  FastLED.show();
  
  // Waiting lights inhale
  for(int x = 55; x < 255; x++)
  {
    for(int i = waitingPosStart; i < waitingPosEnd; i++)
    {
      leds[i] = CHSV(8, 255, x);
    }
    if(digitalRead(in1) == HIGH && digitalRead(in2) == HIGH)
    {
      FastLED.show();
      delay(12);
    }
    else
    {
      break;
    }
  }
  
  // Waiting lights exhale
  for(int x = 255; x > 55; x--)
  {
    for(int i = waitingPosStart; i < waitingPosEnd; i++)
    {
      leds[i] = CHSV(8, 255, x);
    }
    if(digitalRead(in1) == HIGH && digitalRead(in2) == HIGH)
    {
      FastLED.show();
      delay(12);
    }
    else
    {
      break;
    }
  }
} 

void dispensingLights()
{
  // Disable waiting lights
  for(int i = waitingPosStart; i < waitingPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 0);
  }
  // Brighten dispensing lights
  for(int i = dispensingPosStart; i < dispensingPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 100);
  }
  // Brighten display lights
  for(int i = displayPosStart; i < displayPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 100);
  }
  FastLED.show();
  delay(500);

  // Dim dispensing lights
  for(int i = dispensingPosStart; i < dispensingPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 50);
  }
  // Dim display lights
  for(int i = displayPosStart; i < displayPosEnd; i++)
  {
    leds[i] = CHSV(8, 255, 70);
  }
  FastLED.show();
  delay(500);
}

void errorLights()
{
  // Flash all red
  for(int i = waitingPosStart; i < displayPosEnd; i++)
  {
    leds[i] = CHSV(0, 255, 100);
  }
  FastLED.show();
  delay(500);

  // Flash all blue
  for(int i = waitingPosStart; i < displayPosEnd; i++)
  {
    leds[i] = CHSV(160, 255, 100);
  }
  FastLED.show();
  delay(500);
  
}
