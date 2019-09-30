#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=test.PARSE.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$SCCOMP_PATH/sccomp -p $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -p $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$SCCOMP_PATH/sccomp -parse $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -parse $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$SCCOMP_PATH/sccomp --parse $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --parse $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
