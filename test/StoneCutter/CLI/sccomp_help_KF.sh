#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -e
retVal=$?
if [[ "$retval" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -e correctly failed with return code = $retVal"
else
  exit 255
fi

$SCCOMP_PATH/sccomp -hlep
retVal=$?
if [[ "$retval" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -hlep correctly failed with return code = $retVal"
else
  exit 255
fi

$SCCOMP_PATH/sccomp --hepl
retVal=$?
if [[ "$retval" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --hepl correctly failed with return code = $retVal"
else
  exit 255
fi

exit 0
