#!/bin/bash

POAR_PATH=$1

$POAR_PATH/poar
retVal=$?
if [ $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar had no input files; it should have returned an error: return=$retVal"
  exit 1
else
  exit 0
fi
