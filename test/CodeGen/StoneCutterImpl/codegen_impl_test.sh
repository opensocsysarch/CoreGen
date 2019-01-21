#!/bin/bash

CGCLI_PATH=$1
FILE=$2
OPTS="--chisel"

if [ ! -f $CGCLI_PATH/cgcli ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

if [ ! -f $FILE ]; then
  echo "COULD NOT FIND YAML INPUT $FILE"
  exit -1
fi

DIR=`grep "ProjectRoot" $FILE | awk '{print $2}'`

$CGCLI_PATH/cgcli $OPTS --ir $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/cgcli $OPTS --ir $FILE failed with return code = $retVal"
  exit $retVal
fi

#rm -Rf $DIR
exit 0
