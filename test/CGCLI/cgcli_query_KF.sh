#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli --query
retVal=$?
if [[  "$retVal" -eq 0 ]]; then
  echo "$CGCLI_PATH/cgcli --query error was not caught"
  exit $retVal
fi

exit 0
