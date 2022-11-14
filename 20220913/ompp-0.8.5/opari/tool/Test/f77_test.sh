#!/bin/sh
TOOL=..
#set -x

for file in test?.f
do
  echo "        $file ..."
  base=`basename $file .f`
  ${TOOL}/opari -table opari.tab.c $base.f || exit
  if diff $base.f.out $base.mod.F > /dev/null
  then
    true
  else
    echo "-------- ERROR: unexpected change in transformed program --------"
    diff $base.f.out $base.mod.F
    exit
  fi
  if diff $base.f.opari.inc.out $base.f.opari.inc > /dev/null
  then
    true
  else
    echo "-------- ERROR: unexpected change in opari include file --------"
    diff $base.f.opari.inc.out $base.f.opari.inc
    exit
  fi
done
