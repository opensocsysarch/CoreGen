#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=chisel.sc

$SCCOMP_PATH/sccomp -c $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -c $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -chisel $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -chisel $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --chisel $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --chisel $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
