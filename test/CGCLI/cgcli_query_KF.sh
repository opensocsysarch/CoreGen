#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli --query
retVal=$?
if [[  "$retVal" -eq 0 ]]; then
  echo "$CGCLI_PATH/cgcli --query error was not caught"
  exit -1
fi

echo "TESTING PASSED"

exit 0
