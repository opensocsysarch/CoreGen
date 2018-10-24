#!/bin/bash

CGCLI_PATH=$1

FILE=test.NOOPT.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$CGCLI_PATH/cgcli -N $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -N $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli -no-optimize $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -no-optimize $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli --no-optimize $FILE
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp --no-optimize $FILE failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
