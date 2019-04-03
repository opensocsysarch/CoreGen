#!/bin/bash

SCCOMP_PATH=$1
FILE=$2

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi


$SCCOMP_PATH/sccomp -p -N $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -p -N $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -parse -no-optimize $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -parse -no-optimize $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --parse --no-optimize $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --parse --no-optimize $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
