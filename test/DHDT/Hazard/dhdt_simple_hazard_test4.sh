#!/bin/bash
# dhdt_simple_hazard_test4.sh

DHDT_PATH=$1
TEST=simple_test4

$DHDT_PATH/dhdt --llvm $TEST.ll --coregen $TEST.yaml --hazard --inst $TEST.inst --output $TEST.csv
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm $TEST.ll --coregen $TEST.yaml --hazard --inst $TEST.inst --output $TEST.csv failed with return code = $retVal"
  exit $retVal
fi
