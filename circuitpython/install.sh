#!/bin/bash

SRC=/Volumes/CIRCUITPY
LIB=$SRC/lib

if [ "$1" == "-f" ]; then
  echo "Cleaning up hidden files..."
  mdutil -i off $SRC > /dev/null
  cd $SRC > /dev/null
  rm -rf .{,_.}{fseventsd,Spotlight-V*,Trashes}
  mkdir .fseventsd
  touch .fseventsd/no_log .metadata_never_index .Trashes
  cd - > /dev/null

  echo "Uploading library files..."
  mkdir -p $LIB
  cp -X lib/adafruit_bno055.mpy $LIB
  cp -X lib/neopixel.mpy $LIB
  cp -rX lib/adafruit_bus_device $LIB
  cp -rX lib/adafruit_register $LIB
  #cp -rX lib/adafruit_hid $LIB
fi

echo "Uploading the source files..."
cd src
cp -X core.py $SRC
cp -X main.py $SRC
cd - > /dev/null

echo "Syncing filesystems..."
sync
