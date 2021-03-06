#!/bin/bash

CONFIG_PATH=$1

$CONFIG_PATH/coregen-config -h
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -h failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -help
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -help failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --help
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --help failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -c
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -c failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -compile
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -compile failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --compile
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --compile failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -i
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -i failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -include
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -include failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --include
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --include failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -I
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -I failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -INCLUDE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -INCLUDE failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --INCLUDE
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --INCLUDE failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -l
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -l failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -libs
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -libs failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --libs
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --libs failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -L
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -L failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -LIBS
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -LIBS failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --LIBS
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --LIBS failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -k
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -L failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -linkpath
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -linkpath failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --linkpath
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --linkpath failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -K
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -K failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -LINKPATH
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -LINKPATH failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --LINKPATH
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --LINKPATH failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -p
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -p failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -plugin
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -plugin failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --plugin
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --plugin failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -P
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -P failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config -PLUGIN
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config -PLUGIN failed with return code = $retVal"
  exit $retVal
fi

$CONFIG_PATH/coregen-config --PLUGIN
retVal=$?
if [[ "$retVal" -ne 0 ]]; then
  echo "$CONFIG_PATH/coregen-config --PLUGIN failed with return code = $retVal"
  exit $retVal
fi


# EOF
