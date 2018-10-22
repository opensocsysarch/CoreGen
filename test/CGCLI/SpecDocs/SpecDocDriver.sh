#!/bin/bash

CGCLI_PATH=$1
YAML_INPUT=$2

f1=${YAML_INPUT%.yaml}
YAML_OUTPUT=${f1##*/}

#-- generate the doc

mkdir -p ./Specs/$YAML_OUTPUT
$CGCLI_PATH/cgcli -i $YAML_INPUT --enable-sys-pass "SpecDoc:./Specs/$YAML_OUTPUT"
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "$CGCLI_PATH/cgcli  failed with return code = $retVal"
  exit $retVal
fi

#-- attempt to build the doc
cd ./Specs/$YAML_OUTPUT && make clean && make
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "Build of spec doc at ./Specs/$YAML_OUTPUT failed with return code = $retVal"
  exit $retVal
fi


exit 0
