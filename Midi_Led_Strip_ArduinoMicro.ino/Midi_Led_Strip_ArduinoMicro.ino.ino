#include "MIDIUSB.h"
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

int header, channel, note, velocity;

void setup() {

  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);


  for (int i = 0; i < NUM_LEDS; i++) {
    rainbowPos[i] = (gHue + HUE_OFFSET) % 255;
    gHue += rainbowDivision;
    //Serial.println(rainbowPos[i]);
  }

}

void loop() {

  MIDIread();

  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

}

void handlennOn(byte _channel, byte number, byte value)
{
  //  Serial.print("channel = ");
  //  Serial.print(_channel);
  //  Serial.print("  number = ");
  //  Serial.print(number);
  //  Serial.print("  value = ");
  //  Serial.println(value);

  int nn = number - 21;
  nn *= 2;
  byte randColor = random(0, 255);

  leds[nn].setHSV(rainbowPos[nn], 255, map(value, 0, 127, 100, 255)); //HSV
  leds[nn + 1].setHSV(rainbowPos[nn + 1], 255, map(value, 0, 127, 100, 255)); //HSV


  //Serial.println(channel);

  switch (_channel) {
    case 1:
      leds[nn].setHSV(rainbowPos[nn], 255, map(value, 0, 127, 100, 255)); //HSV
      leds[nn + 1].setHSV(rainbowPos[nn + 1], 255, map(value, 0, 127, 100, 255)); //HSV
      break;
    case 2:
      leds[nn].setHue(randColor);
      leds[nn + 1].setHue(randColor);
      break;
    case 3:
      leds[nn] = CRGB::Blue;
      leds[nn + 1] = CRGB::Blue;
      break;
    case 4:
      leds[nn] = CRGB::Green;
      leds[nn + 1] = CRGB::Green;
      break;
    case 5:
      leds[nn] = CRGB::Red;
      leds[nn + 1] = CRGB::Red;
      break;
    case 6:
      leds[nn] = CRGB::HotPink;
      leds[nn + 1] = CRGB::HotPink;
      break;
    case 7:
      leds[nn] = CRGB::Purple;
      leds[nn + 1] = CRGB::Purple;
      break;
    case 8:
      leds[nn] = CRGB::Aqua;
      leds[nn + 1] = CRGB::Aqua;
      break;
    case 9:
      leds[nn] = CRGB::Aquamarine;
      leds[nn + 1] = CRGB::Aquamarine;
      break;
    case 10:
      leds[nn] = CRGB::Coral;
      leds[nn + 1] = CRGB::Coral;
      break;
    case 11:
      leds[nn] = CRGB::DarkOrange;
      leds[nn + 1] = CRGB::DarkOrange;
      break;
    case 12:
      leds[nn] = CRGB::DeepPink;
      leds[nn + 1] = CRGB::Blue;
      break;
    case 13:
      leds[nn] = CRGB::Magenta;
      leds[nn + 1] = CRGB::Green;
      break;
    case 14:
      leds[nn] = CRGB::Red;
      leds[nn + 1] = CRGB::Cyan;
      break;
    case 15:
      leds[nn] = CRGB::Green;
      leds[nn + 1] = CRGB::Yellow;
      break;
    case 16:
      leds[nn] = CRGB::White;
      leds[nn + 1] = CRGB::White;
      break;

  }

  //leds[nn].setRGB( 255, 68, 221); //RGB
  //leds[nn + 1] = CRGB::HotPink;
}

void handlennOff(byte channel, byte number, byte value)
{
  int nn = number - 21;
  nn *= 2;
  leds[nn].setRGB(0, 0, 0);
  leds[nn + 1].setRGB(0, 0, 0);


}

void handleControlChange(byte channel, byte number, byte value)
{

}

void printNotes() {
  Serial.print("header = ");
  Serial.print(header);
  Serial.print("  channel = ");
  Serial.print(channel);
  Serial.print("  note = ");
  Serial.print(note);
  Serial.print("  velocity = ");
  Serial.println(velocity);
}

//void MIDIread() {
//
//  midiEventPacket_t rx;
//  rx = MidiUSB.read();
//
//  if (rx.header != 0) {
//    header = rx.header;
//    channel = rx.byte1;
//    note = rx.byte2;
//    velocity = rx.byte3;
//
//    printNotes();
//
//    handlennOn(channel, note, velocity);
//    handlennOff(channel, note, velocity);
//  }
//}

void MIDIread() {

midiEventPacket_t rx = MidiUSB.read();

  switch (rx.header) {
    case 0:
      break; //No pending events

    case 0x9: // note On
      rx.byte1 = channel;  //channel
      rx.byte2 = note;       //pitch
      rx.byte3 = velocity;       //velocity
      break;
    case 0x8:
      rx.byte1 = channel;  //channel
      rx.byte2 = note;       //pitch
      rx.byte3 = velocity;       //velocity
      break;
      break;

    default:
      Serial.print("Unhandled MIDI message: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
  }
}

  /////////////////////////////////////////////
  // Arduino (pro)micro midi functions MIDIUSB Library
  void nnOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t nnOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(nnOn);
  }

  void nnOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t nnOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(nnOff);
  }

  void controlChange(byte channel, byte control, byte value) {
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
  }

