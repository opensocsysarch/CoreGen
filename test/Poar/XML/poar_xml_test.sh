#!/bin/bash

POAR_PATH=$1
FILE=$2

$POAR_PATH/poar --design $FILE --outfile $FILE.out.xml --xml
if [[  "$retVal" -ne 0 ]]; then
  echo "$POAR_PATH/poar --design $FILE --outfile $FILE.out.xml --xml failed with return code = $retVal"
  exit $retVal
fi

$POAR_PATH/poar --sigmap $FILE
if [[  "$retVal" -ne 0 ]]; then
  echo "$POAR_PATH/poar --sigmap $FILE --overhead 15 failed with return code = $retVal"
  exit $retVal
fi

exit 0
