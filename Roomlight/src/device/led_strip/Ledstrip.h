#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#include "../colors/Colors.h"

class Ledstrip {
private:
  int i_countLeds;
  int i_brightness;
  int i_colorModus;
  boolean b_stripState;

  Adafruit_NeoPixel* p_strip;

  uint32_t getColorFromColorCode(int*);
  uint32_t getColorFromColorCode(int*, int);
  void dimmenToValue(int, int*, int);
public:
  Ledstrip(int i_pin, int i_countLeds, int i_brightness, int i_colorModus=0):
   i_countLeds(i_countLeds),
   i_brightness(i_brightness),
   i_colorModus(i_colorModus)
   {
     switch(i_colorModus){
       case MODUS_RGBW:
        p_strip = new Adafruit_NeoPixel(i_countLeds, i_pin, NEO_GRBW + NEO_KHZ800);
        break;
      default:
        p_strip = new Adafruit_NeoPixel(i_countLeds, i_pin, NEO_GRB + NEO_KHZ800);
        break;
     }
     setStatus(false);
   }

  void init();

  void setBrightness(int);
  int getBrightness();

  void setStatus(boolean);
  boolean getStatus();

  int getLEDCount();

  void toColor(int*);
  void fade(int*, char, int);
  void multiFade(int**, int, char, int);
  void dimmen(int, int*, int);
  void rainbow(char, int);
};
#endif //_LEDSTRIP_H_
