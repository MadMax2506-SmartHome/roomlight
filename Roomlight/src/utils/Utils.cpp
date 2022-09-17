#include "Utils.h"

char* stringToChar(String s_str) {
  const int i_size = s_str.length() + 5;
  char* p_ptr = new char[i_size];
  s_str.toCharArray(p_ptr, i_size);
  return p_ptr;
}