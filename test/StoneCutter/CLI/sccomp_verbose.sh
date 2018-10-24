#!/bin/bash

CGCLI_PATH=$1

FILE=test.VERBOSE.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$CGCLI_PATH/cgcli -v $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -v $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli -verbose $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -verbose $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli --verbose $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp --verbose $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
