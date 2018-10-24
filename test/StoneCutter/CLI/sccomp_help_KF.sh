#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli -e
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/sccomp -e correctly  failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli -hlep
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/sccomp -hlep correctly failed with return code = $retVal"
  exit $retVal
fi

$CGCLI_PATH/cgcli --hepl
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/sccomp --hepl correctly failed with return code = $retVal"
  exit $retVal
fi

exit 0
