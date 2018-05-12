#!/bin/bash

SRC=/Volumes/CIRCUITPY
LIB=$SRC/lib

if [ "$1" == "-l" ]; then
  echo "Uploading library files..."
  mkdir -p $LIB
  cp lib/adafruit_bno055.mpy $LIB
  cp lib/neopixel.mpy $LIB
  cp -r lib/adafruit_bus_device $LIB
  cp -r lib/adafruit_register $LIB
  #cp -r lib/adafruit_hid $LIB
fi

echo "Uploading the source files..."
cd src
cp core.py $SRC
cp main.py $SRC
cd - > /dev/null
