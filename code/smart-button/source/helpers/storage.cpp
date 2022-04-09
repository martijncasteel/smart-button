#include <EEPROM.h>
// #include "storage.h"

/**
 * @file storage.cpp
 * @date 2022-04-09
 * 
 * @brief Helper class for reading and writing to eeprom.
 * 
 */

class Storage {
  public:
    static String read_string_from_eeprom(int address) {
      int length = EEPROM.read(address);
      char data[length + 1];

      for ( int i = 0; i < length; i++ ) {
        data[i] = EEPROM.read(address + i + 1);
      }

      data[length] = '\0';
      return data;
    }

    static void write_string_to_eeprom(int address, const char * string) {
      int length = strlen(string);

      EEPROM.write(address, length);

      for (int i = 0; i < length; i++) {
        EEPROM.write(address + i + 1, string[i]);
      }
    }
};