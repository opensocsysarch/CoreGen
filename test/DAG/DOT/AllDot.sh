#!/bin/bash

DOTFILES=`ls -lh ./testBin/*.dot | awk '{print $9}'`

for FILE in $DOTFILES;do
  dot -Tpng $FILE > `echo $FILE | sed 's/dot/png/g'`
  #dot -Tps2 $FILE > `echo $FILE | sed 's/dot/png/g'`
done;
