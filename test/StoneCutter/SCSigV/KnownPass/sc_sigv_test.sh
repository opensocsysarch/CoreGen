#!/bin/bash

SCSIGV_PATH=$1
FILE=$2

if [ ! -f $SCSIGV_PATH/scsigv ]; then
  echo "COULD NOT FIND $SCSIGV_PATH/scsigv"
  exit -1
fi


$SCSIGV_PATH/scsigv $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv -s $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv -s $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv -stats $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv -stats $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv --stats $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv --stats $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv -p $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv -p $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv -pipeline $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv -pipeline $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv --pipeline $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv --pipeline $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv -p -s $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv -p -s $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv -pipeline -stats $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv -pipeline -stats $FILE failed with return code = $retVal"
  exit $retVal
fi

$SCSIGV_PATH/scsigv --pipeline --stats $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCSIGV_PATH/scsigv --pipeline --stats $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0

#-- EOF
