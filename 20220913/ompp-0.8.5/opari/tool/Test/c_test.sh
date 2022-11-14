#!/bin/sh
TOOL=..
#set -x

for file in test?.c
do
  echo "        $file ..."
  base=`basename $file .c`
  ${TOOL}/opari $base.c || exit
  if diff $base.c.out $base.mod.c > /dev/null
  then
    true
  else
    echo "-------- ERROR: unexpected change in transformed program --------"
    diff $base.c.out $base.mod.c
    exit
  fi
  if diff $base.c.opari.inc.out $base.c.opari.inc > /dev/null
  then
    true
  else
    echo "-------- ERROR: unexpected change in opari include file --------"
    diff $base.c.opari.inc.out $base.c.opari.inc
    exit
  fi
done
