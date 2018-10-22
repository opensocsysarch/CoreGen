#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli --list-sys-passes
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli --list-sys-passes failed with return code = $retVal"
  exit $retVal
fi

exit 0
