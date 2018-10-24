#!/bin/bash

CGCLI_PATH=$1

FILE=test.PARSE.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$CGCLI_PATH/cgcli -p $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -p $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli -parse $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -parse $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli --parse $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp --parse $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
