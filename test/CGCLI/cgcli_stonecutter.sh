#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -i stonecutter.yaml --stonecutter
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -i stonecutter.yaml --stonecutter failed with return code = $retVal"
  exit $retVal
fi

echo "TESTING PASSED"

exit 0
