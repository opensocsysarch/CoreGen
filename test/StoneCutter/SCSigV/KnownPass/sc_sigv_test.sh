#!/bin/bash

SCSIGV_PATH=$1
FILE=$2

if [ ! -f $SCSIGV_PATH/scsigv ]; then
  echo "COULD NOT FIND $SCSIGV_PATH/scsigv"
  exit -1
fi


$SCSIGV_PATH/scsigv $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
