#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -e
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/cgcli -h erroneously passed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli -hlep
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/cgcli -help erroneously passed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --hlep
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/cgcli --help erroneously passed with return code = $retVal"
  exit $retVal
fi

echo "TESTING PASSED"

exit 0
