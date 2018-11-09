#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=sc_parser_test2.sc

$SCCOMP_PATH/sccomp -p $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -p $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -parse $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp -parse $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --parse $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$SCCOMP_PATH/sccomp --parse $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
