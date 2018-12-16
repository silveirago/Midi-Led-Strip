#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();


#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    176
#define HUE_OFFSET 90
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
//////////////////////////////////////

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t rainbowPos[NUM_LEDS] = {0};
uint8_t rainbowDivision = 255 / NUM_LEDS;

void setup() {

  //MIDI.begin();
  Serial.begin (31250);

  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  //  Serial.print("rainbow division = ");
  //  Serial.println(rainbowDivision);
  //  Serial.println();

  for (int i = 0; i < NUM_LEDS; i++) {
    rainbowPos[i] = (gHue + HUE_OFFSET) % 255;
    gHue += rainbowDivision;
    Serial.println(rainbowPos[i]);
  }

}

void loop() {

  MIDI.read();

  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  //  int radomNumber = random(0, 40);
  //  strip.setPixelColor(radomNumber, 255, 255, 255);
  //  strip.show();
  //  delay(100);
  //  strip.setPixelColor(radomNumber, 0, 0, 0);
  //  strip.show();
  //  delay(100);

}

void handleNoteOn(byte channel, byte number, byte value)
{
  int note = number - 21;
  note *= 2;
  byte randColor = random(0, 255);

  switch (channel) {
    case 1:
      leds[note].setHSV(rainbowPos[note], 255, map(value, 0, 127, 100, 255)); //HSV
      leds[note + 1].setHSV(rainbowPos[note + 1], 255, map(value, 0, 127, 100, 255)); //HSV
      break;
    case 2:    
      leds[note].setHue(randColor);
      leds[note + 1].setHue(randColor);
      break;
    case 3:
      leds[note] = CRGB::Blue;
      leds[note + 1] = CRGB::Blue;
      break;
    case 4:
      leds[note] = CRGB::Green;
      leds[note + 1] = CRGB::Green;
      break;
    case 5:
      leds[note] = CRGB::Red;
      leds[note + 1] = CRGB::Red;
      break;
    case 6:
      leds[note] = CRGB::HotPink;
      leds[note + 1] = CRGB::HotPink;
      break;
    case 7:
      leds[note] = CRGB::Purple;
      leds[note + 1] = CRGB::Purple;
      break;
    case 8:
      leds[note] = CRGB::Aqua;
      leds[note + 1] = CRGB::Aqua;
      break;
    case 9:
      leds[note] = CRGB::Aquamarine;
      leds[note + 1] = CRGB::Aquamarine;
      break;
    case 10:
      leds[note] = CRGB::Coral;
      leds[note + 1] = CRGB::Coral;
      break;
    case 11:
      leds[note] = CRGB::DarkOrange;
      leds[note + 1] = CRGB::DarkOrange;
      break;
    case 12:
      leds[note] = CRGB::DeepPink;
      leds[note + 1] = CRGB::Blue;
      break;
    case 13:
      leds[note] = CRGB::Magenta;
      leds[note + 1] = CRGB::Green;
      break;
    case 14:
      leds[note] = CRGB::Red;
      leds[note + 1] = CRGB::Cyan;
      break;
    case 15:
      leds[note] = CRGB::Green;
      leds[note + 1] = CRGB::Yellow;
      break;
    case 16:
      leds[note] = CRGB::White;
      leds[note + 1] = CRGB::White;
      break;

  }

  //leds[note].setRGB( 255, 68, 221); //RGB
  //leds[note + 1] = CRGB::HotPink;
}

void handleNoteOff(byte channel, byte number, byte value)
{
  int note = number - 21;
  note *= 2;
  leds[note].setRGB(0, 0, 0);
  leds[note + 1].setRGB(0, 0, 0);


}

void handleControlChange(byte channel, byte number, byte value)
{


}



