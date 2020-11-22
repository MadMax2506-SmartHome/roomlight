#include "AbstractStorage.h";

boolean AbstractStorage::b_initEEPROM = false;

boolean AbstractStorage::isInRange(int i_place) {
  //prüfen, ob Speicherstelle im reservierten Bereich ist
  return (i_place >= i_startStorageIndex && i_place <= i_endStorageIndex) ? true : false;
}

boolean AbstractStorage::isValid(int i_value) {
  //prüfen, ob Wert zwischen 0 und 255 liegt
  return (i_value >= 0 && i_value <= 255) ? true : false;
}
