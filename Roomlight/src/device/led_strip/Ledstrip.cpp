#include "Ledstrip.h"

uint32_t Ledstrip::getColorFromColorCode(int* pi_colorCode) {
  //prüfen, ob rgb ledstrip
  if(i_colorModus == MODUS_RGB) {
    //rgb ledstrip

    return p_strip->Color(pi_colorCode[0], pi_colorCode[1], pi_colorCode[2]);
  } else {
    //rgbw ledstrip

    return p_strip->Color(pi_colorCode[0], pi_colorCode[1], pi_colorCode[2], pi_colorCode[3]);
  }
}

void Ledstrip::setStatus(boolean b_stripState) {
  Ledstrip::b_stripState = b_stripState;
}

boolean Ledstrip::getStatus() {
  return b_stripState;
}

int Ledstrip::getLEDCount() {
  return i_countLeds;
}

void Ledstrip::init() {
  int pi_colorCode[] = {0, 0, 0, 0};

  p_strip->begin();

  setBrightness(i_brightness);
  toColor(pi_colorCode);

  p_strip->show();
}

void Ledstrip::setBrightness(int i_brightness) {
  p_strip->setBrightness(i_brightness);
  p_strip->show();
}

int Ledstrip::getBrightness() {
  return i_brightness;
}

void Ledstrip::toColor(int* pi_colorCode) {
  p_strip->fill(getColorFromColorCode(pi_colorCode));
  p_strip->show();
}

void Ledstrip::fade(int* pi_colorCode, char c_orientation, int i_waitAfterEachLed) {
  uint32_t i_colorCode = getColorFromColorCode(pi_colorCode);

  switch (c_orientation) {
    //von rechts nach nach links
    case 'l':
      //wiederholt für alle leds
      for(int i = i_countLeds-1; i >= 0; i--) {
        p_strip->setPixelColor(i, i_colorCode);
        p_strip->show();

        delay(i_waitAfterEachLed);
      }
      break;
    //von links nach rechts
    case 'r':
      //wiederholt für alle leds
      for(int i = 0; i < i_countLeds; i++) {
        p_strip->setPixelColor(i, i_colorCode);
        p_strip->show();

        delay(i_waitAfterEachLed);
      }
      break;
    //von der mitte ausgehend
    default:
      int i_middle = i_countLeds/2;

      //prüfen, ob anzahl der leds ungerade ist
      if((i_countLeds % 2) == 0) {
        //anzahl der leds ist gerade

        p_strip->setPixelColor(i_middle, i_colorCode);
        p_strip->setPixelColor(i_middle - 1, i_colorCode);

        //wiederholt für die hälfte der leds
        for(int i = 1; i < i_middle; i++) {
          p_strip->setPixelColor(i_middle + i, i_colorCode);
          p_strip->setPixelColor(i_middle - i - 1 , i_colorCode);
          p_strip->show();

          delay(i_waitAfterEachLed);
        }
      } else {
        //anzahl der leds ist ungerade

        p_strip->setPixelColor(i_middle, i_colorCode);

        //wiederholt für alle leds
        for(int i = 1; i <= i_middle; i++) {
          p_strip->setPixelColor(i_middle + i, i_colorCode);
          p_strip->setPixelColor(i_middle - i , i_colorCode);
          p_strip->show();

          delay(i_waitAfterEachLed);
        }
      }
      break;
  }
}

void Ledstrip::multiFade(int** ppi_colorCode, int i_countColors, char c_orientation, int i_waitAfterEachLed) {
  int i_index = 0;

  switch (c_orientation) {
    //von rechts nach links
    case 'l':
      //wiederholt für alle leds
      for(int i = i_countLeds-1; i >= 0; i--) {
        p_strip->setPixelColor(i, getColorFromColorCode(ppi_colorCode[i_index]));
        p_strip->show();

        i_index++;
        if(i_index == i_countColors) {
          i_index = 0;
        }

        delay(i_waitAfterEachLed);
      }
      break;
    //von links nach rechts
    case 'r':
      //wiederholt für alle leds
      for(int i = 0; i < i_countLeds; i++) {
        p_strip->setPixelColor(i, getColorFromColorCode(ppi_colorCode[i_index]));
        p_strip->show();

        i_index++;
        if(i_index == i_countColors) {
          i_index = 0;
        }

        delay(i_waitAfterEachLed);
      }
      break;
    //von der mitte ausgehend
    default:
      int i_middle = i_countLeds/2;

      //prüfen, ob anzahl der leds gerade ist
      if((i_countLeds % 2) == 0) {
        //anzahl der leds is gerade

        p_strip->setPixelColor(i_middle, getColorFromColorCode(ppi_colorCode[i_index]));
        p_strip->setPixelColor(i_middle-1, getColorFromColorCode(ppi_colorCode[i_index]));

        i_index++;
        if(i_index == i_countColors) {
          i_index = 0;
        }

        //wiederholt für die hälfte der leds
        for(int i = 1; i < i_middle; i++) {
          p_strip->setPixelColor(i_middle + i, getColorFromColorCode(ppi_colorCode[i_index]));
          p_strip->setPixelColor(i_middle - i - 1, getColorFromColorCode(ppi_colorCode[i_index]));
          p_strip->show();
          i_index++;
          if(i_index == i_countColors) {
            i_index = 0;
          }
          delay(i_waitAfterEachLed);
        }
      } else {
        //anzahl der leds ist ungerade

        p_strip->setPixelColor(i_middle, getColorFromColorCode(ppi_colorCode[i_index]));

        i_index++;
        if(i_index == i_countColors) {
          i_index = 0;
        }

        //wiederholt für die hälfte der leds
        for(int i = 1; i <= i_middle; i++) {
          p_strip->setPixelColor(i_middle + i, getColorFromColorCode(ppi_colorCode[i_index]));
          p_strip->setPixelColor(i_middle - i, getColorFromColorCode(ppi_colorCode[i_index]));
          p_strip->show();

          i_index++;
          if(i_index == i_countColors) {
            i_index = 0;
          }

          delay(i_waitAfterEachLed);
        }
      }
      break;
  }
}

void Ledstrip::dimmen(int i_newBrightness, int i_waitAfterEachLed) {
  int i_oldBrightness = p_strip->getBrightness();
  
  if(i_oldBrightness < i_newBrightness) {
    // Heller werden
    for(int i = i_oldBrightness; i < i_newBrightness; i++) {
      dimmenToValue(i, i_waitAfterEachLed);
    }
  } else if (i_oldBrightness > i_newBrightness) {
    // Dunkler werden
    for(int i = i_oldBrightness; i > i_newBrightness; i--) {
      dimmenToValue(i, i_waitAfterEachLed);
    }
  }
}

void Ledstrip::dimmenToValue(int value, int i_waitAfterEachLed) {
  //prüfen, ob rgb ledstrip
  if(i_colorModus == MODUS_RGB) {
    //rgb ledstrip
    p_strip->fill(p_strip->Color(p_strip->gamma8(value), p_strip->gamma8(value), p_strip->gamma8(value)));
  } else {
    //rgbw ledstrip
    p_strip->fill(p_strip->Color(p_strip->gamma8(value), p_strip->gamma8(value), p_strip->gamma8(value), p_strip->gamma8(value)));
  }

  p_strip->show();

  delay(i_waitAfterEachLed);
}

void Ledstrip::rainbow(char c_orientation, int i_waitAfterEachLed) {
  int i_firstPixelHue = 0;
  switch (c_orientation) {
    //von rechts nach links
    case 'l':
      //wiederholt für alle leds
      for(int i = i_countLeds-1; i >= 0; i--) {
        int i_pixelHue = i_firstPixelHue + (i * 65536L / i_countLeds);

        p_strip->setPixelColor(i, p_strip->gamma32(p_strip->ColorHSV(i_pixelHue)));
        p_strip->show();

        delay(i_waitAfterEachLed);
      }
      break;
    //von links nach rechts
    default:
      //wiederholt für alle leds
      for(int i = 0; i < i_countLeds; i++) {
        int i_pixelHue = i_firstPixelHue + (i * 65536L / i_countLeds);

        p_strip->setPixelColor(i, p_strip->gamma32(p_strip->ColorHSV(i_pixelHue)));
        p_strip->show();

        delay(i_waitAfterEachLed);
      }
      break;
  }
}
