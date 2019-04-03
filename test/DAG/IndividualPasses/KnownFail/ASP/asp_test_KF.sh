#!/bin/bash

CGCLI_PATH=$1
FILE=$2

if [ ! -f $CGCLI_PATH/cgcli ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

echo "Executing $CGCLI_PATH/cgcli --asp "$FILE.lp" --ir $FILE.yaml"

FSTATUS=`$CGCLI_PATH/cgcli --asp "$FILE.lp" --ir $FILE.yaml | grep "ASPSolverPass : ........................FAILED"`

if [ -z "$FSTATUS" ]
then
  echo "ASPPASSED"
  exit -1
else
  echo "ASPFAILED"
  exit 0
fi

