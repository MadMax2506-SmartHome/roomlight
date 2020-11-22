#ifndef _STORAGE_ELEMENT_H_
#define _STORAGE_ELEMENT_H_

#include <Arduino.h>

#include "AbstractStorage.h"

class StorageElement : public AbstractStorage{
private:
  int i_defaultValue;
public:
  StorageElement(int i_storageIndex, int i_defaultValue = 0):
  AbstractStorage(i_storageIndex, i_storageIndex),
  i_defaultValue(i_defaultValue)
  {}

  int getIndex();

  void reload();
};
#endif //_STORAGE_ELEMENT_H_
