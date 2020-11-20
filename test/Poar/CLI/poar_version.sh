#!/bin/bash

POAR_PATH=$1

$POAR_PATH/poar -v
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar -v failed with return code = $retVal"
  exit $retVal
fi

$POAR_PATH/poar -version
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar -version failed with return code = $retVal"
  exit $retVal
fi

$POAR_PATH/poar --version
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar --version failed with return code = $retVal"
  exit $retVal
fi

exit 0
