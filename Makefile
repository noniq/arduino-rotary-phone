ARDMK_DIR     = /usr/local/opt/arduino-mk

BOARD_TAG     = uno
#ARDUINO_QUIET = 1
#ARDUINO_LIBS = EEPROM FastLED
#CPPFLAGS = -DDEV
ISP_PROG = usbtiny

include ${ARDMK_DIR}/Arduino.mk

