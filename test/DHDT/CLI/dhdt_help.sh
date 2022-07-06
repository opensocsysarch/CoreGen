#!/bin/bash

DHDT_PATH=$1

$DHDT_PATH/dhdt -h
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt -h failed with return code = $retVal"
  exit $retVal
fi

$DHDT_PATH/dhdt -help
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/dhdt -help failed with return code = $retVal"
  exit $retVal
fi

$DHDT_PATH/dhdt --help
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --help failed with return code = $retVal"
  exit $retVal
fi

exit 0
