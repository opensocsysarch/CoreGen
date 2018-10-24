#!/bin/bash

CGCLI_PATH=$1

FILE=test.KEEP.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$CGCLI_PATH/cgcli -k $FILE -o $FILE.out
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -k $FILE -o $FILE.out failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli -keep $FILE -outfile $FILE.out
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp -keep $FILE -outfile $FILE.out failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli --keep $FILE --outfile $FILE.out
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/sccomp --keep $FILE --outfile $FILE.out failed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
