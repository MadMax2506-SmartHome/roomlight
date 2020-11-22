#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <Arduino.h>

#include "AbstractStorage.h"
#include "StorageElement.h"
#include "StorageElementMulti.h"

class Storage : public AbstractStorage {
private:
  int i_crcStorageIndex;

  int getCrcRead();
  int getCrcCalc();
public:
  Storage(int i_crcStorageIndex, int i_startStorageIndex, int i_endStorageIndex):
  AbstractStorage(i_startStorageIndex, i_endStorageIndex),
  i_crcStorageIndex(i_crcStorageIndex)
  {}

  void setCrc();
  boolean compareCrcs();

  int read(StorageElement*);
  int* read(StorageElementMulti*);

  void write(StorageElement*, int);
  void write(StorageElementMulti*, int*);

  boolean commit();
};

#endif //_STORAGE_H
