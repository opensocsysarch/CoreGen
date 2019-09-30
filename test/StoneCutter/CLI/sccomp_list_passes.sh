#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp --list-passes
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --list-passes failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
