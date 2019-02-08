#!/bin/bash

SCCOMP_PATH=$1
FILE=$2

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -c $FILE
retVal=$?
if [[  "$retVal" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -c $FILE was successful; should have failed return code = $retVal"
  rm -Rf $FILE.chisel
  exit -1
fi

rm -Rf $FILE.chisel

exit 0
