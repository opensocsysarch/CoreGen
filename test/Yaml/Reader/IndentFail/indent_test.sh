#!/bin/bash

CGCLI_PATH=$1
FILE=$2

if [ ! -f $CGCLI_PATH/cgcli ]; then
  echo "COULD NOT FIND $CGCLI_PATH/cgcli"
  exit -1
fi

$CGCLI_PATH/cgcli --verify --ir $FILE
retVal=$?
if [[  "$retVal" -eq 0 ]]; then
  echo "$CGCLI_PATH/cgcli --verify --ir $FILE did not catch the indentation issue"
  exit -1
fi

echo "$CGCLI_PATH/cgcli --verify --ir $FILE successfully caught the indentation issue"

exit 0
