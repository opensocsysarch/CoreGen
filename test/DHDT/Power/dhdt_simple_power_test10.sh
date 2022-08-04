#!/bin/bash
# dhdt_simple_power_test10.sh

DHDT_PATH=$1
TEST=simple_test10

$DHDT_PATH/dhdt --llvm $TEST.ll --coregen $TEST.yaml --power $TEST.power.yaml --inst $TEST.inst --output $TEST.csv
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm $TEST.ll --coregen $TEST.yaml --power $TEST.power.yaml --inst $TEST.inst  --output $TEST.csv failed with return code = $retVal"
  exit $retVal
fi
