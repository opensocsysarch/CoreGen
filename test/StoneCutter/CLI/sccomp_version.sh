#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -v
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -v failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -verbose
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -verbose failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --verbose
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp --verbose failed with return code = $retVal"
  exit $retVal
fi

exit 0
