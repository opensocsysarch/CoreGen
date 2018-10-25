#!/bin/bash

CGCLI_PATH=$1

FILE=test.KEEP.sc
touch $FILE
echo "# this is a stonecutter source file" >> $FILE 2>&1

$CGCLI_PATH/cgcli -k $FILE -o
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/sccomp -k $FILE -o passed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli -keep $FILE -outfile
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/sccomp -keep $FILE -outfile passed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

$CGCLI_PATH/cgcli --keep $FILE --outfile
retVal=$?
if [ ! $? -ne 255 ]; then
  echo "$CGCLI_PATH/sccomp --keep $FILE --outfile passed with return code = $retVal"
  rm -Rf $FILE
  exit $retVal
fi

rm -Rf $FILE

exit 0
