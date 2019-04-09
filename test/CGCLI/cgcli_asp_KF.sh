#!/bin/bash

CGCLI_PATH=$1

if [ ! -f $CGCLI_PATH/cgcli ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

echo "Executing $CGCLI_PATH/cgcli --asp "foo.lp" --ir sample1.yaml"

$CGCLI_PATH/cgcli --asp "foo.lp" --ir sample1.yaml
retVal=$?
if [ ! $? -eq 0 ]; then
  echo "Failed to catch the error in erroneous ASP file"
  exit -1
fi
echo "Successfully caught the failed ASP file test"
exit 0

