#!/bin/bash

CGCLI_PATH=$1
TEST=archive_test_3

mkdir $TEST
$CGCLI_PATH/cgcli --archive ./$TEST --init-entry $TEST.yaml TEST1
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli --archive ./$TEST --init-entry $TEST.yaml TEST1 failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --archive ./$TEST --destroy-entry $TEST.yaml TEST1
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli --archive ./$TEST --destroy-entry $TEST.yaml TEST1 failed with return code = $retVal"
  exit $retVal
fi

rm -Rf ./$TEST

exit 0
