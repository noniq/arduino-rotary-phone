ARDMK_DIR     = /usr/local/opt/arduino-mk

BOARD_TAG     = uno
#ARDUINO_QUIET = 1
#ARDUINO_LIBS = EEPROM FastLED
#CPPFLAGS = -DDEV
F_CPU = 8000000L
ISP_PROG = usbtiny

include ${ARDMK_DIR}/Arduino.mk

