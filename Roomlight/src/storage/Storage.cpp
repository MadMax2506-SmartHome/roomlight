#include "Storage.h"

int Storage::getCrcRead() {
  return EEPROM.read(i_crcStorageIndex);
}

int Storage::getCrcCalc() {
  unsigned char uc_calc = 0;
  for(int i = i_startStorageIndex; i <= i_endStorageIndex; i++) {
    uc_calc+= EEPROM.read(i);
  }
  return uc_calc;
}

void Storage::setCrc() {
  EEPROM.write(i_crcStorageIndex, getCrcCalc());
  EEPROM.commit();
}

boolean Storage::compareCrcs() {
  return getCrcRead() == getCrcCalc();
}

int Storage::read(StorageElement* p_elem) {
  //prüfen, ob Speicherplatz gültig ist
  if(!isInRange(p_elem->getIndex())) {
    return 0;
  }
  //Speicher lesen
  return EEPROM.read(p_elem->getIndex());
}

int* Storage::read(StorageElementMulti* p_elem) {
  int i_count = 0;

  int* pi_indices = p_elem->getIndices();
  int* pi_values  = new int[pi_indices[1] - pi_indices[0] + 1];
  for(int i = pi_indices[0]; i <= pi_indices[1]; i++) {
    //Speicher lesen
    pi_values[i_count] = EEPROM.read(i);
    i_count++;
  }
  return pi_values;
}

void Storage::write(StorageElement* p_elem, int i_value) {
  //prüfen, ob der Speicherplatz und der Wert gültig ist
  if(!isInRange(p_elem->getIndex()) || !isValid(i_value)) {
    return;
  }

  //in Speicher schreiben
  EEPROM.write(p_elem->getIndex(), i_value);
}

void Storage::write(StorageElementMulti* p_elem, int* pi_values) {
  int count = 0;

  int* pi_indices = p_elem->getIndices();

  for(int i = pi_indices[0]; i <= pi_indices[1]; i++) {
    //in Speicher schreiben
    EEPROM.write(i, pi_values[count]);
    count++;
  }
}

boolean Storage::commit() {
  //Speicherplätze überschreiben
  setCrc();
  boolean b_result = EEPROM.commit();
  //debug prints
  if(b_result) {
    Serial.println("\n\n--------------------------------------------------");
    Serial.println("The storage was written to successfully");
    Serial.println("--------------------------------------------------\n");
  } else {
    Serial.println("\n\n--------------------------------------------------");
    Serial.println("The storage was not written to successfully");
    Serial.println("--------------------------------------------------\n");
  }
  return b_result;
}
