#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -v
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -v failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli -version
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -version failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --version
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli --version failed with return code = $retVal"
  exit $retVal
fi

exit 0
