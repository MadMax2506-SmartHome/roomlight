#ifndef _ABSTRACTSTORAGE_H_
#define _ABSTRACTSTORAGE_H_

#include <Arduino.h>
#include <EEPROM.h>

class AbstractStorage {
protected:
  static boolean b_initEEPROM;
  int i_startStorageIndex;
  int i_endStorageIndex;

  boolean isInRange(int);
  boolean isValid(int);
public:
  AbstractStorage(int i_startStorageIndex, int i_endStorageIndex):
  i_startStorageIndex(i_startStorageIndex),
  i_endStorageIndex(i_endStorageIndex)
  {
    EEPROM.begin(1024);
    if(!b_initEEPROM) {

      b_initEEPROM = true;
    }
  }
};
#endif //_ABSTRACTSTORAGE_H_
