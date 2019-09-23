#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=sigmap.sc
DIR=test.SIGMAP.yaml

$SCCOMP_PATH/sccomp -s $DIR $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -s $DIR $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -sigmap $DIR $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -sigmap $DIR $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --sigmap $DIR $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --sigmap $DIR $FILE failed with return code = $retVal"
  exit $retVal
fi

rm -Rf $DIR

exit 0
