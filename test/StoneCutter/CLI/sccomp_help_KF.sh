#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -e
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$SCCOMP_PATH/sccomp -e correctly  failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -hlep
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$SCCOMP_PATH/sccomp -hlep correctly failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --hepl
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$SCCOMP_PATH/sccomp --hepl correctly failed with return code = $retVal"
  exit $retVal
fi

exit 0
