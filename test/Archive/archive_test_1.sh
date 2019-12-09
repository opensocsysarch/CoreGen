#!/bin/bash

CGCLI_PATH=$1
TEST=archive_test_1

mkdir $TEST
$CGCLI_PATH/cgcli --archive ./$TEST --query $TEST.yaml
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli --archive ./$TEST --query $TEST.yaml failed with return code = $retVal"
  exit $retVal
fi

rm -Rf ./$TEST

exit 0
