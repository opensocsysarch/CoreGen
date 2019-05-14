#!/bin/bash

SCCOMP_PATH=$1
FILE=$2

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi


$SCCOMP_PATH/sccomp -s $FILE.yaml $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -s $FILE.yaml $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
