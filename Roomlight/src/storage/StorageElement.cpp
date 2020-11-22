#include "StorageElement.h"

int StorageElement::getIndex() {
  return i_startStorageIndex;
}

void StorageElement::reload() {
  //prüfen, ob der Speicherplatz und der Wert gültig ist
  if(!isInRange(i_startStorageIndex) || !isValid(i_defaultValue)) {
    return;
  }

  //in Speicher schreiben
  EEPROM.write(i_startStorageIndex, i_defaultValue);
  EEPROM.commit();
}
