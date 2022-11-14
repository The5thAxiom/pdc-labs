#!/bin/sh
TOOL=..
#set -x

for file in test?.f90
do
  echo "        $file ..."
  base=`basename $file .f90`
  ${TOOL}/opari $base.f90 || exit
  if diff $base.f90.out $base.mod.F90 > /dev/null
  then
    true
  else
    echo "-------- ERROR: unexpected change in transformed program --------"
    diff $base.f90.out $base.mod.F90
    exit
  fi
  if diff $base.f90.opari.inc.out $base.f90.opari.inc > /dev/null
  then
    true
  else
    echo "-------- ERROR: unexpected change in opari include file --------"
    diff $base.f90.opari.inc.out $base.f90.opari.inc
    exit
  fi
done
