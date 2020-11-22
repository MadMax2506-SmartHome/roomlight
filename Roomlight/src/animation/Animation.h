#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <Arduino.h>

#include "../storage/Storage.h"

#include "../led_strip/Ledstrip.h"
#include "../colors/Colors.h"

class Animation {
private:
  int* pi_color;
  int i_time;
  char c_type;
  char c_orientation;
  boolean b_isChange;
  boolean b_status;

  StorageElementMulti* p_colorStorage;
  StorageElement* p_animationTimeStorage;
  StorageElement* p_animationTypeStorage;
  StorageElement* p_orientationStorage;
  StorageElement* p_statusStorage;

  Storage* p_storage;
  Ledstrip* p_strip;
  Colors* p_color;
public:
  Animation(Ledstrip* p_strip, Colors* p_color, int i_crcStorageIndex, int i_startStorageIndex, int i_endStorageIndex) :
  p_strip(p_strip),
  p_color(p_color)
  {
    p_strip->init();

    p_storage = new Storage(i_crcStorageIndex, i_startStorageIndex, i_endStorageIndex);

    p_colorStorage          = new StorageElementMulti(i_startStorageIndex + 0, i_startStorageIndex + 2, new int[3]{255, 255, 255});
    p_animationTimeStorage  = new StorageElement(i_startStorageIndex + 3, 0);
    p_animationTypeStorage  = new StorageElement(i_startStorageIndex + 4, 'f');
    p_orientationStorage    = new StorageElement(i_startStorageIndex + 5, 'r');
    p_statusStorage         = new StorageElement(i_startStorageIndex + 6, true);

    readConf();
  }

  void reloadConf();
  void readConf();
  void writeConf();
  char* getConfAsJSON();

  void setTime(int);
  void setOrientation(char);
  void setType(char);
  void setColor(int*);
  void setStatus(boolean);

  boolean getStatus();

  void animate();
  void restart();
  void idle();
};
#endif //_ANIMATION_H_
