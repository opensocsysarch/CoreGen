#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -cv
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -cv failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli -coregenversion
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -coregenversion failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --coregen-version
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli --coregen-version failed with return code = $retVal"
  exit $retVal
fi

echo "TESTING PASSED"

exit 0
