#!/bin/bash
prog_name=build/RenjuSGFParserTest

if [ ! -e ${prog_name} ]
then
  echo ${prog_name} is not found.　1>&2
  exit 1
fi

./${prog_name}
exit $?
