#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -i sample1.yaml --asp "ASP TEST"
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -i sample1.yaml --asp \"ASP TEST\" failed with return code = $retVal"
  exit $retVal
fi

exit 0
