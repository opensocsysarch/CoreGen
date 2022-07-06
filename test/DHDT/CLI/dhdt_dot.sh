#!/bin/bash

DHDT_PATH=$1

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml -d dhdt_sample.dot
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml -d dhdt_sample.dot failed with return code = $retVal"
  exit $retVal
fi

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml -dot dhdt_sample.dot
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$POAR_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml -dot dhdt_sample.dot failed with return code = $retVal"
  exit $retVal
fi

$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --dot dhdt_sample.dot
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "$DHDT_PATH/dhdt --llvm dhdt_sample.ll --coregen dhdt_sample.yaml --dot dhdt_sample.dot failed with return code = $retVal"
  exit $retVal
fi

dot -Tpdf dhdt_sample.dot > dhdt_sample.pdf
retVal=$?
if [ ! $retVal -eq 0 ]; then
  echo "Failed to generate dhdt pdf from dot input"
  rm dhdt_sample.dot dhdt_sample.pdf
  exit $retVal
fi

rm dhdt_sample.dot dhdt_sample.pdf

exit 0
