#!/bin/bash

POAR_PATH=$1
YAML_INPUT=$2

f1=${YAML_INPUT%.yaml}
YAML_OUTPUT=${f1##*/}

mkdir -p ./Specs/$YAML_OUTPUT

#-- generate the doc

$POAR_PATH/poar --design $YAML_INPUT --outfile ./Specs/$YAML_OUTPUT --latex
if [[  "$retVal" -ne 0 ]]; then
  echo "$POAR_PATH/poar --design $YAML_INPUT --outfile ./Specs/$YAML_OUTPUT --latex failed with return code = $retVal"
  exit $retVal
fi

#-- attempt to build the doc
cd ./Specs/$YAML_OUTPUT && make clean && make
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "Build of POAR doc at ./Specs/$YAML_OUTPUT failed with return code = $retVal"
  exit $retVal
fi

exit 0
