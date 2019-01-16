#!/bin/bash

SCCOMP_PATH=$1
OUTFILE=$2
FILES="sc_parser_test1.sc sc_parser_test2.sc"

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi


$SCCOMP_PATH/sccomp -p -N -f $OUTFILE $FILES
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -p -N -f $OUTFILE $FILES failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -parse -no-optimize -outfile $OUTFILE $FILES
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -parse -no-optimize -outfile $OUTFILE $FILES failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --parse --no-optimize --outfile $OUTFILE $FILES
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --parse --no-optimize --outfile $OUTFILE $FILES failed with return code = $retVal"
  exit $retVal
fi

exit 0
