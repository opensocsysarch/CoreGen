#!/bin/bash

CGCLI_PATH=$1
TEST=archive_test_8

mkdir $TEST
$CGCLI_PATH/cgcli --archive ./$TEST --init $TEST.yaml
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli --archive ./$TEST --init $TEST.yaml failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --archive ./$TEST --destroy $TEST.yaml
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli --archive ./$TEST --destroy $TEST.yaml failed with return code = $retVal"
  exit $retVal
fi

rm -Rf ./$TEST

exit 0
