#!/bin/bash

DHDT_PATH=$1

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --hazard
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --hazard failed with return code = $retVal"
  exit $retVal
fi

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --hazard
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --hazard failed with return code = $retVal"
  exit $retVal
fi

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --hazard
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --hazard failed with return code = $retVal"
  exit $retVal
fi

exit 0
