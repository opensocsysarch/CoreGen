#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -i sample1.yaml --opt --disable-all-passes
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -i sample1.yaml --opt --disable-all-passes failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli -ir sample1.yaml --opt --disable-all-passes
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli -ir sample1.yaml --opt --disable-all-passes failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --ir sample1.yaml --opt --disable-all-passes
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli --ir sample1.yaml --opt --disable-all-passes failed with return code = $retVal"
  exit $retVal
fi

echo "TESTING PASSED"

exit 0
