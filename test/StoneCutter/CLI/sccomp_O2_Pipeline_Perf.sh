#!/bin/bash

SCCOMP_PATH=$1

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

FILE=opt_O2_Pipeline_Perf.sc

$SCCOMP_PATH/sccomp -O2 -Pipeline:Opt=Perf $FILE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -O2 -Pipeline:Opt=Perf $FILE failed with return code = $retVal"
  exit $retVal
fi

exit 0
