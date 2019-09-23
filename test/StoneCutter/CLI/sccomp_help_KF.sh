#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -e
retVal=$?
if [[ "$retVal" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -e was not caught as an error"
  exit 255
else
  echo "$SCCOMP_PATH/sccomp -e correctly failed with return code = $retVal"
fi

$SCCOMP_PATH/sccomp -hlep
retVal=$?
if [[ "$retVal" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -hlep was not caught as an error"
  exit 255
else
  echo "$SCCOMP_PATH/sccomp -hlep correctly failed with return code = $retVal"
fi

$SCCOMP_PATH/sccomp --hepl
retVal=$?
if [[ "$retVal" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --hepl was not caught as an error"
  exit 255
else
  echo "$SCCOMP_PATH/sccomp --hepl correctly failed with return code = $retVal"
fi

exit 0
