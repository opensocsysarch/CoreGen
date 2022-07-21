#!/bin/bash

DHDT_PATH=$1
PCONF=dhdt_power_config_KF1.yaml

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --power $PCONF
retVal=$?
if [ $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --power $PCONF failed to catch bad input; code = $retVal"
  exit -1
fi

exit 0
