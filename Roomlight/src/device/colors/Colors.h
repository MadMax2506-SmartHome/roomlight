#ifndef _COLORS_H_
#define _COLORS_H_

#define MODUS_RGB 0
#define SIZE_RGB 3

#define MODUS_RGBW 1
#define SIZE_RGBW 4

#include <Arduino.h>

class Colors {
private:
  int i_modus;
public:
  Colors(int i_modus = MODUS_RGB):
  i_modus(i_modus)
  {}

  int* white();
  int* red();
  int* green();
  int* blue();
  int* black();
  int* getMix(int, int, int);
  int* getMix(int, int, int, int);
};
#endif //_COLORS_H_
