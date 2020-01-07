#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=./sccomp_disable_all_sc.sc

PASSES=`$SCCOMP_PATH/sccomp --list-sc-passes | grep "Pass" | grep "-" | awk '{print $2}'`

for P in $PASSES;do
  echo "...testing pass=$P"
  $SCCOMP_PATH/sccomp --disable-sc-pass "$P" $FILE
  retVal=$?
  if [[ "$retVal" -ne 0 ]]; then
    echo "$SCCOMP_PATH/sccomp --disable-sc-pass "$P" $FILE failed with return code = $retVal"
    exit $retVal
  fi
done;

exit 0
