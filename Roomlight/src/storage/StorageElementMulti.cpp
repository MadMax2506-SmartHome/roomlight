#include "StorageElementMulti.h"

int* StorageElementMulti::getIndices() {
  return new int[2]{i_startStorageIndex, i_endStorageIndex};
}

void StorageElementMulti::reload() {
  int count = 0;

  for(int i = i_startStorageIndex; i <= i_endStorageIndex; i++) {
    //prüfen, ob der Speicherplatz und der Wert gültig ist
    if(!isInRange(i) || !isValid(pi_defaultValue[count])) {
      return;
    }

    //in Speicher schreiben
    EEPROM.write(i, pi_defaultValue[count]);
    EEPROM.commit();
    count++;
  }
}
