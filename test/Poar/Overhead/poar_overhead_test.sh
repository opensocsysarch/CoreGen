#!/bin/bash

POAR_PATH=$1
FILE=$2

$POAR_PATH/poar --design $FILE --overhead 15
if [[  "$retVal" -ne 0 ]]; then
  echo "$POAR_PATH/poar --design $FILE --overhead 15 failed with return code = $retVal"
  exit $retVal
fi

exit 0
