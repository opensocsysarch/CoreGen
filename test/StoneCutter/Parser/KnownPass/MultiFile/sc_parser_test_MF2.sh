#!/bin/bash

SCCOMP_PATH=$1
FILES="sc_parser_test3.sc sc_parser_test4.sc sc_parser_test5.sc sc_parser_test6.sc"

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi


$SCCOMP_PATH/sccomp -p -N $FILES
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -p -N $FILES failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp -parse $FILES
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -parse -no-optimize $FILES failed with return code = $retVal"
  exit $retVal
fi

$SCCOMP_PATH/sccomp --parse $FILES
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp --parse --no-optimize $FILES failed with return code = $retVal"
  exit $retVal
fi

exit 0
