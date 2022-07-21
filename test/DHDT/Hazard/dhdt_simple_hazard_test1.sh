#!/bin/bash
# dhdt_simple_hazard_test1.sh

DHDT_PATH=$1
TEST=simple_test1

$DHDT_PATH/dhdt --llvm $TEST.ll --coregen $TEST.yaml --hazard --inst $TEST.inst
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm $TEST.ll --coregen $TEST.yaml --hazard --inst $TEST.inst failed with return code = $retVal"
  exit $retVal
fi
