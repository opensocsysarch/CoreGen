#!/bin/bash

SCCOMP_PATH=$1
FILE=$2

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -p -N $FILE
retVal=$?
if [ $retVal -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -p -N $FILE failed with return code = $retVal"
  exit -1
fi

exit 0
