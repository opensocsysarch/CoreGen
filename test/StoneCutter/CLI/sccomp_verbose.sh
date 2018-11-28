#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=test.VERBOSE.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$SCCOMP_PATH/sccomp -v $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -v $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$SCCOMP_PATH/sccomp -verbose $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -verbose $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$SCCOMP_PATH/sccomp --verbose $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp --verbose $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
