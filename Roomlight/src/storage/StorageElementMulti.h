#ifndef _STORAGE_ELEMENT_MULTI_H_
#define _STORAGE_ELEMENT_MULTI_H_

#include <Arduino.h>

#include "AbstractStorage.h"

class StorageElementMulti : public AbstractStorage{
private:
  int* pi_defaultValue;
public:
  StorageElementMulti(int i_startStorageIndex, int i_endStorageIndex, int* pi_defaultValue  = NULL):
  AbstractStorage(i_startStorageIndex, i_endStorageIndex),
  pi_defaultValue(pi_defaultValue)
  {}

  int* getIndices();

  void reload();
};
#endif //_STORAGE_ELEMENT_MULTI_H_
