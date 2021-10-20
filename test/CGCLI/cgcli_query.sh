#!/bin/bash

CGCLI_PATH=$1

$CGCLI_PATH/cgcli --query sample_archive.yaml
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli --query sample_archive.yaml failed with return code = $retVal"
  exit $retVal
fi

echo "TESTING PASSED"

exit 0
