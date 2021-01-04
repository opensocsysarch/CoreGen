#!/bin/bash

POAR_PATH=$1

$POAR_PATH/poar -h
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar -h failed with return code = $retVal"
  exit $retVal
fi

$POAR_PATH/poar -help
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar -help failed with return code = $retVal"
  exit $retVal
fi

$POAR_PATH/poar --help
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/poar --help failed with return code = $retVal"
  exit $retVal
fi

exit 0
