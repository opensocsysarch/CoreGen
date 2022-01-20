#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -h
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -h failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli -help
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -help failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --help
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli --help failed with return code = $retVal"
  exit $retVal
fi

echo "TESTING PASSED"

exit 0
