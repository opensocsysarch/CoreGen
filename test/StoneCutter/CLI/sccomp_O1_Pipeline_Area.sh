#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=opt_O1_Pipeline_Area.sc

$SCCOMP_PATH/sccomp -O1 -Pipeline:Opt=Area $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -O1 -Pipeline:Opt=Area $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
