#!/bin/bash

SCCOMP_PATH=$1
FILE=sc_vliw_pipeline_stage_KF.sc
SIGMAP=sc_vliw_pipeline_stage_KF.yaml

if [ ! -f $SCCOMP_PATH/sccomp ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

$SCCOMP_PATH/sccomp -D --sigmap $SIGMAP --verbose -Pipeline $FILE
retVal=$?
if [[  "$retVal" -eq 0 ]]; then
  echo "$SCCOMP_PATH/sccomp -D --sigmap $SIGMAP --verbose -Pipeline $FILE passed when it should have failed"
  echo "Return code = $retVal"
fi

exit 0
