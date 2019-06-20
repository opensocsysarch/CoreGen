#!/bin/bash

CGCLI_PATH=$1
SCCOMP_PATH=$2
FILE=$3
OPTS="--chisel"

if [ ! -f $CGCLI_PATH/cgcli ]; then
  echo "COULD NOT FIND $SCCOMP_PATH/sccomp"
  exit -1
fi

if [ ! -f $FILE ]; then
  echo "COULD NOT FIND YAML INPUT $FILE"
  exit -1
fi

DIR=`grep "ProjectRoot" $FILE | awk '{print $2}'`

$CGCLI_PATH/cgcli $OPTS --ir $FILE
retVal=$?
if [[  "$retVal" -ne 0 ]]; then
  echo "$CGCLI_PATH/cgcli $OPTS --ir $FILE failed with return code = $retVal"
  exit $retVal
fi

ISAs=`grep "ISAName" $FILE | awk '{print $3}' | uniq`

#-- walk through all the stonecutter files and attempt to compile them
for ISA in $ISAs; do
  if [ -f $DIR/RTL/stonecutter/$ISA.sc ]; then
    #-- generate the signal map
    $SCCOMP_PATH/sccomp -s $DIR/RTL/stonecutter/$ISA.yaml $DIR/RTL/stonecutter/$ISA.sc
    retValSm=$?
    if [[  "$retValSm" -ne 0 ]]; then
      echo "$SCCOMP_PATH/sccomp -s $DIR/RTL/stonecutter/$ISA.yaml $DIR/RTL/stonecutter/$ISA.sc failed with return code = $retValSc"
      exit $retValSc
    fi

    #-- run the stonecutter codegen
    $SCCOMP_PATH/sccomp -p $DIR/RTL/stonecutter/$ISA.sc
    retValSc=$?
    if [[  "$retValSc" -ne 0 ]]; then
      echo "$SCCOMP_PATH/sccomp -p $DIR/RTL/stonecutter/$ISA.sc failed with return code = $retValSc"
      exit $retValSc
    fi
  fi
done;

rm -Rf $DIR
exit 0
