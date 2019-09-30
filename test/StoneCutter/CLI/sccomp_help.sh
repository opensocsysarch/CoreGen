#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -h
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -h failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -help
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -help failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --help
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --help failed with return code = $retVal"
  exit $retVal
fi

exit 0
