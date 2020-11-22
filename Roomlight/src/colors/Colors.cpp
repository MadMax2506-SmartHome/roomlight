#include "Colors.h"

int* Colors::white() {
  if(i_modus == MODUS_RGB) {
    return new int[SIZE_RGB]{255, 255, 255};
  } else {
    return new int[SIZE_RGBW]{255, 255, 255, 255};
  }
}

int* Colors::red() {
  if(i_modus == MODUS_RGB) {
    return new int[SIZE_RGB]{255, 0, 0};
  } else {
    return new int[SIZE_RGBW]{255, 0, 0, 0};
  }
}

int* Colors::green() {
  if(i_modus == MODUS_RGB) {
    return new int[SIZE_RGB]{0, 255, 0};
  } else {
    return new int[SIZE_RGBW]{0, 255, 0, 0};
  }
}

int* Colors::blue() {
  if(i_modus == MODUS_RGB) {
    return new int[SIZE_RGB]{0, 0, 255};
  } else {
    return new int[SIZE_RGBW]{0, 0, 255, 0};
  }
}

int* Colors::black() {
  if(i_modus == MODUS_RGB) {
    return new int[SIZE_RGB]{0, 0, 0};
  } else {
    return new int[SIZE_RGBW]{0, 0, 0, 0};
  }
}

int* Colors::getMix(int i_red, int i_green, int i_blue) {
  return new int[SIZE_RGB]{i_red, i_green, i_blue};
}

int* Colors::getMix(int i_red, int i_green, int i_blue, int i_white) {
  return new int[SIZE_RGBW]{i_red, i_green, i_blue, i_white};
}
